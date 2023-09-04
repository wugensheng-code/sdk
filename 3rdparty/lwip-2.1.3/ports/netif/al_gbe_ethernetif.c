#include "al_gbe_ethernetif.h"
#include "al_gbe_hal.h"

#include "lwipopts.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/snmp.h"

#include <string.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'A'
#define IFNAME1 'L'

#if !NO_SYS
#include "arch/sys_arch.h"
#include "lwip/sys.h"

#include "lwip/tcpip.h"

sys_sem_t GbeRxSem;
sys_sem_t GbeTxSem;
#endif /* !NO_SYS */

/* Tx and Rx descriptors define, AL_GBE_RX_DESC_CNT and AL_GBE_TX_DESC_CNT at least four */
AL_GBE_DMADescStruct DMARxDescList[AL_GBE_RX_DESC_CNT] __attribute__ ((aligned (4)));
AL_GBE_DMADescStruct DMATxDescList[AL_GBE_TX_DESC_CNT] __attribute__ ((aligned (4)));

/* Rx descriptors buffer, use static buffer, just user Rx descriptors buffer1 */
uint8_t RxBuffTab[AL_GBE_RX_DESC_CNT][ETH_RX_BUFFER_SIZE] __attribute__ ((aligned (4)));

/* Netif for lwip */
struct netif gnetif;

/* Gbe driver handle */
AL_GBE_HalStruct GbeHandle;

/* Tx config, Configure the Tx descriptor function according to this */
AL_GBE_TxDescConfigStruct TxConfig;

/* define RX_POOL, for Rx zero copy */
LWIP_MEMPOOL_DECLARE(RX_POOL, 48, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");

void pbuf_free_custom(struct pbuf *p)
{
    struct pbuf_custom* custom_pbuf = (struct pbuf_custom*)p;
    /* invalidate data cache: lwIP and/or application may have written into buffer */
    //ToDo: invalidate data cache here
    LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL;
    AL_GBE_BufferStruct RxBuff;
    uint32_t framelength = 0;
    struct pbuf_custom* custom_pbuf;

    if(AlGbe_Hal_GetRxDataBuffer(&GbeHandle, &RxBuff) == AL_OK)
    {
        AlGbe_Hal_GetRxDataLength(&GbeHandle, &framelength);

        /* Build Rx descriptor to be ready for next data reception */
        AlGbe_Hal_BuildRxDescriptors(&GbeHandle);

        /* Invalidate data cache for ETH Rx Buffers */
        //ToDo: invalidate data cache here

        custom_pbuf  = (struct pbuf_custom*)LWIP_MEMPOOL_ALLOC(RX_POOL);
        if (custom_pbuf == NULL) {
            return NULL;
        }

        custom_pbuf->custom_free_function = pbuf_free_custom;

        p = pbuf_alloced_custom(PBUF_RAW, framelength, PBUF_REF, custom_pbuf, RxBuff.Buffer, ETH_RX_BUFFER_SIZE);

    }

    return p;
}

void AlGbe_TxDoneCallback(void *CallbackRef)
{
#if !NO_SYS
    rt_interrupt_enter();
    sys_sem_signal(&GbeTxSem);
    rt_interrupt_leave();
#endif
}

void AlGbe_RxDoneCallback(void *CallbackRef)
{
#if !NO_SYS
    rt_interrupt_enter();
    sys_sem_signal(&GbeRxSem);
    rt_interrupt_leave();
#endif
}

void AlGbe_TxFreeCallback(void *buff)
{
    pbuf_free((struct pbuf *)buff);
}

#if !NO_SYS
/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet was sent, or ERR_IF if the packet was unable to be sent
 *
 * @note ERR_OK means the packet was sent (but not necessarily transmit complete),
 * and ERR_IF means the packet has more chained buffers than what the interface supports.
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    uint32_t i = 0U;
    struct pbuf *q = NULL;
    err_t errval = ERR_OK;
    AL_GBE_BufferStruct Txbuffer[AL_GBE_TX_DESC_CNT];

    memset(Txbuffer, 0, AL_GBE_TX_DESC_CNT*sizeof(AL_GBE_BufferStruct));

    for (q = p; q != NULL; q = q->next)
    {
        if(i >= AL_GBE_TX_DESC_CNT)
            return ERR_IF;

        Txbuffer[i].Buffer = q->payload;
        Txbuffer[i].Len = q->len;

        if(i > 0)
        {
            Txbuffer[i-1].next = &Txbuffer[i];
        }

        if(q->next == NULL)
        {
            Txbuffer[i].next = NULL;
        }

        i++;
    }

    TxConfig.Length = p->tot_len;
    TxConfig.TxBuffer = Txbuffer;
    TxConfig.pData = p;

    pbuf_ref(p);

    AlGbe_Hal_TransmitBlock(&GbeHandle, &TxConfig, 10);

    sys_arch_sem_wait(&GbeTxSem, 0);

    AlGbe_Hal_ReleaseTxPacket(&GbeHandle);

    return errval;
}

/**
 * This task should be signaled when a receive packet is ready to be read
 * from the interface.
 *
 * @param argument the lwip network interface structure for this ethernetif
 */
static void ethernetif_input( void const * argument )
{
    struct pbuf *p = NULL;
    struct netif *netif = (struct netif *) argument;

    for( ;; )
    {
        if ((sys_arch_sem_wait(&GbeRxSem, 0)) != SYS_ARCH_TIMEOUT)
        {
            do
            {
                p = low_level_input( netif );
                if (p != NULL)
                {
                    if (netif->input( p, netif) != ERR_OK )
                    {
                        pbuf_free(p);
                    }
                }

            } while(p != NULL);
        }
    }
}

#else

void ethernetif_input(struct netif *netif)
{
  struct pbuf *p = NULL;

    do
    {
        p = low_level_input( netif );
        if (p != NULL)
        {
            if (netif->input( p, netif) != ERR_OK )
            {
                pbuf_free(p);
            }
        }

    } while(p != NULL);
}

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    uint32_t i = 0U;
    struct pbuf *q = NULL;
    err_t errval = ERR_OK;
    AL_GBE_BufferStruct Txbuffer[AL_GBE_TX_DESC_CNT] = {0};

    memset(Txbuffer, 0 , AL_GBE_TX_DESC_CNT*sizeof(AL_GBE_BufferStruct));

    for(q = p; q != NULL; q = q->next)
    {
        if(i >= AL_GBE_TX_DESC_CNT)
            return ERR_IF;

        Txbuffer[i].Buffer = q->payload;
        Txbuffer[i].Len = q->len;

        if(i > 0)
        {
            Txbuffer[i-1].next = &Txbuffer[i];
        }

        if(q->next == NULL)
        {
            Txbuffer[i].next = NULL;
        }

        i++;
    }

    TxConfig.Length = p->tot_len;
    TxConfig.TxBuffer = Txbuffer;
    TxConfig.pData = p;

    AlGbe_Hal_Transmit(&GbeHandle, &TxConfig);

    return errval;
}

#endif /* !NO_SYS */

#if !NO_SYS
sys_thread_t eth_handle = NULL;
#endif /* !NO_SYS */

err_t low_level_phy_init(AL_GBE_HalStruct *GbeHandle, struct netif *netif, AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_S32 ret;
    AL_U8 speed;
    AL_U8 duplex;
    AL_U8 linkchange = 0;

    /* Initialize the ETH PHY */
    ret = AlGbe_Hal_PhyInit(GbeHandle, GBE_PHY_ADDR);
    if (ret != 0)
    {
        printf("AlGbe_Hal_PhyInit Init failed\r\n");
        return ERR_IF;
    }

    ret = AlGbe_Hal_GetPhyLinkStatus(GbeHandle, GBE_PHY_ADDR, &speed, &duplex);
    if (ret != 0)
    {
        netif_set_link_down(netif);
        netif_set_down(netif);
        printf("AlGbe_Hal_GetPhyLinkStatus link down\r\n");
        return ERR_IF;
    }

    if (PHY_SPEED_10M == speed)
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_10M)
        {
            GbeHandle->Dev->MacDmaConfig.Speed = AL_GBE_SPEED_10M;
            linkchange = 1;
        }
    }
    else if (PHY_SPEED_100M == speed)
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_100M)
        {
            GbeHandle->Dev->MacDmaConfig.Speed = AL_GBE_SPEED_100M;
            linkchange = 1;
        }
    }
    else
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_1G)
        {
            GbeHandle->Dev->MacDmaConfig.Speed = AL_GBE_SPEED_1G;
            linkchange = 1;
        }
    }

    if (PHY_FULL_DUPLEX == duplex)
    {
        if (MacDmaConfig->DuplexMode != AL_GBE_FULL_DUPLEX_MODE)
        {
            GbeHandle->Dev->MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
            linkchange = 1;
        }
    }
    else
    {
        if (MacDmaConfig->DuplexMode != AL_GBE_HALF_DUPLEX_MODE)
        {
            GbeHandle->Dev->MacDmaConfig.DuplexMode = AL_GBE_HALF_DUPLEX_MODE;
            linkchange = 1;
        }
    }

    if (linkchange)
    {
        AlGbe_Hal_ConfigDuplexAndSpeed(GbeHandle);
    }

#if NO_SYS
    AlGbe_Hal_StartMacDma(GbeHandle);
#else
    AlGbe_Hal_StartMacDmaIntr(GbeHandle);
#endif
    netif_set_up(netif);
    netif_set_link_up(netif);

    return ERR_OK;
}

err_t low_level_init(struct netif *netif)
{
    AL_S32 ret;
    AL_U8 MAcAddr[6]= {GBE_MAC_ADDR0, GBE_MAC_ADDR1, GBE_MAC_ADDR2, GBE_MAC_ADDR3, GBE_MAC_ADDR4, GBE_MAC_ADDR5};
    AL_GBE_InitStruct InitConfig;
    AL_GBE_MacDmaConfigStruct MacDmaConfig;

    /* Initialize Gbe driver first */
    InitConfig.MacAddr[0] = MAcAddr[0];
    InitConfig.MacAddr[1] = MAcAddr[1];
    InitConfig.MacAddr[2] = MAcAddr[2];
    InitConfig.MacAddr[3] = MAcAddr[3];
    InitConfig.MacAddr[4] = MAcAddr[4];
    InitConfig.MacAddr[5] = MAcAddr[5];
    InitConfig.MediaInterface = AL_GBE_RGMII_MODE;
    InitConfig.RxDescList = DMARxDescList;
    InitConfig.TxDescList = DMATxDescList;
    InitConfig.RxBuffLen = ETH_RX_BUFFER_SIZE;

    MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
    MacDmaConfig.Speed = AL_GBE_SPEED_100M;

    ret = AlGbe_Hal_Init(&GbeHandle, GBE_DEVICE_ID, &InitConfig, &MacDmaConfig, AL_NULL);
    if (ret != AL_OK)
    {
        printf("AlGbe_Hal_Init failed\r\n");
        return ERR_IF;
    }

    /* Use static buffer to config rx descriptor buffer */
    for (int idx = 0; idx < AL_GBE_RX_DESC_CNT; idx ++)
    {
        AlGbe_Hal_ConfigRxDescBuffer(&GbeHandle, idx, RxBuffTab[idx], NULL);
    }

    AlGbe_Hal_RegisterIntrHandlerCallBack(&GbeHandle, AL_GBE_INTR_TX_COMPLETE, AlGbe_TxDoneCallback);
    AlGbe_Hal_RegisterTxFreeCallBack(&GbeHandle, AlGbe_TxFreeCallback);
    AlGbe_Hal_RegisterIntrHandlerCallBack(&GbeHandle, AL_GBE_INTR_RX_COMPLETE, AlGbe_RxDoneCallback);

    /* Initialize the RX POOL */
    LWIP_MEMPOOL_INIT(RX_POOL);

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = MAcAddr[0];
    netif->hwaddr[1] = MAcAddr[1];
    netif->hwaddr[2] = MAcAddr[2];
    netif->hwaddr[3] = MAcAddr[3];
    netif->hwaddr[4] = MAcAddr[4];
    netif->hwaddr[5] = MAcAddr[5];

    /* maximum transfer unit */
    netif->mtu = ETH_MAX_PAYLOAD;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* Set Tx packet config common parameters */
    memset(&TxConfig, 0 , sizeof(AL_GBE_TxDescConfigStruct));
    TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

#if !NO_SYS
    /* create a binary semaphore used for Tx and Rx done */
    ret = sys_sem_new(&GbeRxSem, 0);
    if (ret != ERR_OK)
    {
        printf("sys_sem_new failed\r\n");
        return ret;
    }
    ret = sys_sem_new(&GbeTxSem, 0);
    if (ret != ERR_OK)
    {
        printf("sys_sem_new failed\r\n");
        return ret;
    }

    eth_handle = sys_thread_new("eth_iput", ethernetif_input, &gnetif, 2048, 8);
#endif /* !NO_SYS */

    ret = low_level_phy_init(&GbeHandle, netif, &MacDmaConfig);
    if (ret != ERR_OK)
    {
        printf("low_level_phy_init failed\r\n");
        return ret;
    }

    printf("AlGbe Init success\r\n");

    return ERR_OK;
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

void netif_config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    /* IP address default setting */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    /* add the network interface */
#if NO_SYS
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
#else
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
#endif

    /* Registers the default network interface */
    netif_set_default(&gnetif);

    printf("Static IP  : %d.%d.%d.%d\r\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    printf("Netmask    : %d.%d.%d.%d\r\n", NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    printf("Gateway    : %d.%d.%d.%d\r\n", GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    printf("Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                         gnetif.hwaddr[0], gnetif.hwaddr[1], gnetif.hwaddr[2],
                         gnetif.hwaddr[3], gnetif.hwaddr[4], gnetif.hwaddr[5]);
}
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 */

/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_ethlite_ethernetif.h"
#include "al_axi_ethernet_lite_hal.h"

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

sys_sem_t EthLiteRxSem;
sys_sem_t EthLiteTxSem;

sys_thread_t ethlite_handle = NULL;

#ifdef RTOS_FREERTOS
extern volatile portBASE_TYPE xInsideISR;
#endif

#define ETH_LITE_RX_QUEUE

#endif /* !NO_SYS */

/* Netif for lwip */
struct netif ethlite_netif;

static AL_AXI_ETHLITE_HalStruct *AXIETHHandle;
static AL_U8 EthLitePhyAddr = 0;

queue_t pq_queue[NUM_QUEUES];
static queue_t *recv_q;
static queue_t *send_q;

char temp_buffer[AL_AXI_ETHLITE_MAX_FRAME_SIZE];

queue_t *queue_create()
{
    static int i;
    queue_t *q = NULL;

    if (i >= NUM_QUEUES) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ERR: Max Queues allocated\n\r"));
        return q;
    }

    q = &pq_queue[i++];

    if (!q)
        return q;

    q->head = q->tail = q->len = 0;

    return q;
}

int queue_send(queue_t *q, void *p)
{
    if (q->len == QUEUE_SIZE)
        return -1;

    q->data[q->head] = p;
    q->head = (q->head + 1)%QUEUE_SIZE;
    q->len++;

    return 0;
}

void *queue_recv(queue_t *q)
{
    int ptail;

    if (q->len == 0)
        return NULL;

    ptail = q->tail;
    q->tail = (q->tail + 1)%QUEUE_SIZE;
    q->len--;

    return q->data[ptail];
}

int queue_get_len(queue_t *q)
{
    return q->len;
}

#if !NO_SYS
static struct pbuf *low_level_input()
{
#ifndef ETH_LITE_RX_QUEUE
    AL_S32 Ret;
    AL_U16 RxFrameLen = 0;

    struct pbuf *p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
    if (p == NULL) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "low_level_input pbuf_alloc null.\r\n");
        return p;
    }

    Ret = AlAxiEthLite_Hal_RecvFrameIntr(AXIETHHandle, (char *)(p->payload), &RxFrameLen);
    if ((Ret == AL_OK) && (RxFrameLen > 0)) {
        return p;
    } else {
        pbuf_free(p);
    }

    return AL_NULL;

#else

    if (queue_get_len(recv_q) == 0)
        return NULL;

    /* return one packet from receive q */
    return (struct pbuf *)queue_recv(recv_q);
#endif
}

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t errval = ERR_OK;
    struct pbuf *q;

    SYS_ARCH_DECL_PROTECT(lev);

    SYS_ARCH_PROTECT(lev);

    if (AlAxiEthLite_Hal_IsTxBufferAvailable(AXIETHHandle) == AL_TRUE) {

        if (queue_get_len(send_q)) {      /* send backlog */
            q = queue_recv(send_q);

            AlAxiEthLite_Hal_SendFrameIntr(AXIETHHandle, q->payload, q->tot_len);

            SYS_ARCH_UNPROTECT(lev);
            pbuf_free(q);

            return ERR_OK;
        } else {                 /* send current */

            AlAxiEthLite_Hal_SendFrameIntr(AXIETHHandle, p->payload, p->tot_len);

            SYS_ARCH_UNPROTECT(lev);
            return ERR_OK;
        }
    } else {
        pbuf_ref(p);

        if (queue_send(send_q, (void *)p) < 0) {

            SYS_ARCH_UNPROTECT(lev);
            return ERR_MEM;
        }
    }

    SYS_ARCH_UNPROTECT(lev);

    return ERR_OK;
}

#ifdef ETH_LITE_RX_QUEUE
static void ethlite_input( void const * argument )
{
    struct pbuf *p = NULL;
    struct netif *netif = (struct netif *) argument;
    SYS_ARCH_DECL_PROTECT(lev);

    for( ;; )
    {
        if ((sys_arch_sem_wait(&EthLiteRxSem, 0)) != SYS_ARCH_TIMEOUT)
        {
            do
            {
                SYS_ARCH_PROTECT(lev);
                p = low_level_input( netif );
                SYS_ARCH_UNPROTECT(lev);
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
#endif

#else

static struct pbuf *low_level_input()
{
    AL_S32 Ret;
    AL_U16 RxFrameLen = 0;

    struct pbuf *p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
    if (p == NULL) {
        return p;
    }

    Ret = AlAxiEthLite_Hal_RecvFrame(AXIETHHandle, p->payload, &RxFrameLen);
    if ((Ret == AL_OK) && (RxFrameLen > 0)) {
        return p;
    } else {
        pbuf_free(p);
    }

    return AL_NULL;
}

void ethlite_ethernetif_input(struct netif *netif)
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
    err_t errval = ERR_OK;

    errval = AlAxiEthLite_Hal_SendFrame(AXIETHHandle, p->payload, p->tot_len);

    return errval;
}

#endif

void AlEthlite_TxDoneCallback(void *CallbackRef)
{
#if !NO_SYS
#if defined(RTOS_RTTHREAD)
    rt_interrupt_enter();
#elif defined(RTOS_FREERTOS)
    xInsideISR++;
#endif

    struct pbuf *p;

    if (queue_get_len(send_q) && (AlAxiEthLite_Hal_IsTxBufferAvailable(AXIETHHandle) == AL_TRUE)) {
        struct pbuf *p = queue_recv(send_q);
        AlAxiEthLite_Hal_SendFrameIntr(AXIETHHandle, p->payload, p->tot_len);
        pbuf_free(p);
    }

#if defined(RTOS_RTTHREAD)
    rt_interrupt_leave();
#elif defined(RTOS_FREERTOS)
    xInsideISR--;
#endif

#endif
}

void AlEthlite_RxDoneCallback(void *CallbackRef)
{
#if !NO_SYS
#if defined(RTOS_RTTHREAD)
    rt_interrupt_enter();
#elif defined(RTOS_FREERTOS)
    xInsideISR++;
#endif

#ifndef ETH_LITE_RX_QUEUE
    struct pbuf * p = low_level_input();
    if (p != NULL)
    {
        if (ethlite_netif.input( p, &ethlite_netif) != ERR_OK )
        {
            pbuf_free(p);
        }
    }
#else
    AL_S32 Ret;
    AL_U16 RxFrameLen = 0;

    struct pbuf *p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
    if (p == NULL) {
        AlAxiEthLite_Hal_RecvFrameIntr(AXIETHHandle, temp_buffer, &RxFrameLen);
#if defined(RTOS_RTTHREAD)
        rt_interrupt_leave();
#elif defined(RTOS_FREERTOS)
        xInsideISR--;
#endif
        return;
    }

    Ret = AlAxiEthLite_Hal_RecvFrameIntr(AXIETHHandle, p->payload, &RxFrameLen);
    if ((Ret != AL_OK) || (RxFrameLen == 0)) {
        pbuf_free(p);
#if defined(RTOS_RTTHREAD)
        rt_interrupt_leave();
#elif defined(RTOS_FREERTOS)
        xInsideISR--;
#endif
        return;
    }

    if (queue_send(recv_q, (void*)p) < 0) {

        pbuf_free(p);
#if defined(RTOS_RTTHREAD)
        rt_interrupt_leave();
#elif defined(RTOS_FREERTOS)
        xInsideISR--;
#endif
        return;
    }

    sys_sem_signal(&EthLiteRxSem);

#endif

#if defined(RTOS_RTTHREAD)
    rt_interrupt_leave();
#elif defined(RTOS_FREERTOS)
    xInsideISR--;
#endif

#endif
}

AL_S32 AlAxiEthLite_GetPhyLinkSpeedDuplex(AL_AXI_ETHLITE_HalStruct *AXIETHHandle, AL_AXI_ETHLITE_SpeedEnum *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;
    AL_U16 RegValue;
    AL_U16 TempSpeed;

    AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, EthLitePhyAddr, PHY_SSR_REG, &RegValue);
    TempSpeed = (RegValue & 0x2);

    switch (TempSpeed)
    {
    case (AL_U32)PHY_SPEED_10M:
        *Speed = AL_AXI_ETHLITE_SPEED_10M;
        break;
    case (AL_U32)PHY_SPEED_100M:
        *Speed = AL_AXI_ETHLITE_SPEED_100M;
        break;
    default:
        *Speed = AL_AXI_ETHLITE_SPEED_10M;
        break;
    }

    if ((RegValue & 0x4) != 0U) {
        *Duplex = PHY_FULL_DUPLEX;
    } else {
        *Duplex = PHY_HALF_DUPLEX;
    }

    return AL_OK;
}

AL_S32 AlAxiEthLite_GetPhyLinkStatus(AL_AXI_ETHLITE_HalStruct *AXIETHHandle, AL_BOOL *Status)
{
    AL_S32 Ret;
    AL_U16 RegValue;

    Ret = AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, EthLitePhyAddr, PHY_SSR_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAxiEthLite_Hal_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }
    RegValue = RegValue & 0x1;

    if (RegValue) {
        *Status = AL_TRUE;
    } else {
        *Status = AL_FALSE;
    }

    return AL_OK;
}

AL_S32 AlAxiEthLite_PhyInit(AL_AXI_ETHLITE_HalStruct *AXIETHHandle)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue = 0;
    AL_U16 TmpRegValue = 0;
    AL_U8 PHYAddress = EthLitePhyAddr;

    /* Reset the Phy */
    Ret = AlAxiEthLite_Hal_WritePhyRegister(AXIETHHandle, PHYAddress, PHY_BCR_REG, PHY_BCTL_RESET_MASK);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAxiEthLite_Hal_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    /* Delay at least 10 ms */
    AlSys_MDelay(100);

    /* Check reset status complete */
    Ret = AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, PHYAddress, PHY_BCR_REG, &RegValue);
    if ((Ret != AL_OK) || (RegValue & PHY_BCTL_RESET_MASK)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_AXI_ETHLITE_ERR_PHY_READ;
    }

    Ret = AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    RegValue |= PHY_ADVERTISE_100_MASK;
    RegValue |= PHY_ADVERTISE_10_MASK;
    Ret = AlAxiEthLite_Hal_WritePhyRegister(AXIETHHandle, PHYAddress, PHY_AUTONEG_ADVERTISE_REG, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAxiEthLite_Hal_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, PHYAddress, PHY_BCR_REG, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    RegValue |= PHY_BCTL_AUTONEG_MASK;
    RegValue |= PHY_BCTL_RESTART_AUTONEG_MASK;
    Ret = AlAxiEthLite_Hal_WritePhyRegister(AXIETHHandle, PHYAddress, PHY_BCR_REG, RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
        return Ret;
    }

    do {
        AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, PHYAddress, PHY_BSR_REG, &RegValue);
    } while (!(RegValue & PHY_BSTATUS_AUTONEGCOMP_MASK));

    return AL_OK;
}

AL_S32 AlAxiEthLite_DetectPhy(AL_AXI_ETHLITE_HalStruct *AXIETHHandle)
{
    AL_S32 Ret;
    AL_S32 Index;
    AL_U16 PhyId1 = 0;

    EthLitePhyAddr = 0;

    for (Index = 1; Index < 0x20; Index++) {
        AlAxiEthLite_Hal_ReadPhyRegister(AXIETHHandle, Index, PHY_ID1_REG, &PhyId1);
        if (PhyId1 != 0xFFFF && PhyId1 != 0x00) {
            EthLitePhyAddr = Index;
            break;
        }

    }

    if (Index == 0x20) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "No Phy device detected.\r\n");
        return AL_AXI_ETHLITE_ERR_PHY_NOT_DETECT;
    }

    return AL_OK;
}

err_t ethlite_low_level_phy_init(AL_AXI_ETHLITE_HalStruct *AXIETHHandle, struct netif *netif)
{
    AL_S32 Ret;
    AL_U8 LinkStatus;
    AL_U8 Speed;
    AL_U8 Duplex;
    AL_U8 LinkChange = 0;

    Ret = AlAxiEthLite_DetectPhy(AXIETHHandle);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAXIETHHandle_Hal_DetectPhy failed\r\n");
        return ERR_IF;
    }

    /* Initialize the ETH PHY */
    Ret = AlAxiEthLite_PhyInit(AXIETHHandle);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAXIETHHandle_Hal_PhyInit Init failed\r\n");
        return ERR_IF;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlAXIETHHandle get phy link status...\r\n");
    Ret = AlAxiEthLite_GetPhyLinkStatus(AXIETHHandle, &LinkStatus);
    if ((Ret != 0) && (LinkStatus != AL_TRUE))
    {
        netif_set_link_down(netif);
        netif_set_down(netif);
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAXIETHHandle phy link down\r\n");
        return ERR_IF;
    }

    Ret = AlAxiEthLite_GetPhyLinkSpeedDuplex(AXIETHHandle, &Speed, &Duplex);
    if (Ret != 0)
    {
        netif_set_link_down(netif);
        netif_set_down(netif);
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlAXIETHHandle phy link down\r\n");
        return ERR_IF;
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "AlAXIETHHandle phy link up success\r\n");

    AlAxiEthLite_Hal_ConfigDuplexAndSpeed(AXIETHHandle, Speed, Duplex);

    return ERR_OK;
}

err_t ethlite_low_level_init(struct netif *netif)
{
    AL_S32 ret;
    AL_U8 MAcAddr[6]= {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};

    AL_AXI_ETHLITE_InitStruct InitConfig = {
        .MacAddr = {MAcAddr[0], MAcAddr[1], MAcAddr[2], MAcAddr[3], MAcAddr[4], MAcAddr[5]},
        .Speed   = AL_AXI_ETHLITE_SPEED_AUTONEG,
    };

    AlAxiEthLite_Hal_Init(&AXIETHHandle, 0, &InitConfig, AL_NULL);
    ethlite_low_level_phy_init(AXIETHHandle, netif);

#if !NO_SYS
   AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlAxiEthLite_Hal_RegisterIntrHandlerCallBack(AXIETHHandle, AL_AXI_ETHLITE_INTR_RX_VALID, AlEthlite_RxDoneCallback);
    AlAxiEthLite_Hal_RegisterIntrHandlerCallBack(AXIETHHandle, AL_AXI_ETHLITE_INTR_TX_DONE, AlEthlite_TxDoneCallback);

    ret = sys_sem_new(&EthLiteRxSem, 0);
    if (ret != ERR_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "sys_sem_new failed\r\n");
        return ret;
    }

    send_q = queue_create();

#ifdef ETH_LITE_RX_QUEUE
    recv_q = queue_create();
    ethlite_handle = sys_thread_new(ETH_INPUT_THREAD_NAME, (lwip_thread_fn)ethlite_input, &ethlite_netif,
                                ETH_INPUT_THREAD_STACKSIZE, ETH_INPUT_THREAD_PRIO);
#endif

    AlAxiEthLite_Hal_EnableIntr(AXIETHHandle);
#endif

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

    netif_set_up(netif);
    netif_set_link_up(netif);

    AL_LOG(AL_LOG_LEVEL_INFO, "AlEthLiteInit success\r\n");

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
err_t ethlite_ethernetif_init(struct netif *netif)
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
  ethlite_low_level_init(netif);

  return ERR_OK;
}

void ethlite_netif_config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    /* IP address default setting */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

#if NO_SYS
    netif_add(&ethlite_netif, &ipaddr, &netmask, &gw, NULL, &ethlite_ethernetif_init, &ethernet_input);
#else
    netif_add(&ethlite_netif, &ipaddr, &netmask, &gw, NULL, &ethlite_ethernetif_init, &tcpip_input);
#endif

    /* Registers the default network interface */
    netif_set_default(&ethlite_netif);

    AL_LOG(AL_LOG_LEVEL_INFO, "Static IP  : %d.%d.%d.%d\r\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    AL_LOG(AL_LOG_LEVEL_INFO, "Netmask    : %d.%d.%d.%d\r\n", NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    AL_LOG(AL_LOG_LEVEL_INFO, "Gateway    : %d.%d.%d.%d\r\n", GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    AL_LOG(AL_LOG_LEVEL_INFO, "Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                         ethlite_netif.hwaddr[0], ethlite_netif.hwaddr[1], ethlite_netif.hwaddr[2],
                         ethlite_netif.hwaddr[3], ethlite_netif.hwaddr[4], ethlite_netif.hwaddr[5]);
}
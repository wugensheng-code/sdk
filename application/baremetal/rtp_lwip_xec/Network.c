//
// Modefy by Jiadong Xiao on 2021/6/20.
//

#include "io.h"
#include <stdio.h>
#include <string.h>
#include "Network.h"

#include "lwip/init.h"
#include "lwip/udp.h"
#include "lwip/stats.h"
#include "lwip/inet_chksum.h"

#define RTPERR (0xffffffff)

struct test_udp_rxdata
{
  u32_t rx_cnt;
  u32_t rx_bytes;
  struct udp_pcb *pcb;
};

static struct netif test_netif1;
static ip4_addr_t test_gw1, test_ipaddr1, test_netmask1;
static int output_ctr, linkoutput_ctr;

struct udp_pcb *send_pcb;
struct test_udp_rxdata ctr1;

static void data_dump_name(struct pbuf *p, char *name)
{
  u16_t cnt, loop;
  u8_t *cur;

  if (NULL == p)
    return;

  printf("-----DUMP %s 0x%x : size %d[%d]-----\r\n", name, (mem_ptr_t)p, p->tot_len, p->len);
  loop = p->tot_len;
  cur = (u8_t *)p->payload;

  if (loop >= 1024)
  {
    printf("too big len %d err!\r\n", loop);
    loop = 1024;
  }

  cnt = 0;
  while (loop--)
  {
    printf("0x%x ,", *cur);
    cnt++;
    if (cnt >= 16)
    {
      printf("\r\n");
      cnt = 0;
    }
    cur++;
  }
  printf("\r\n---------DUMP END--------\r\n");
}

/* 10.8.9.4 */
/* 8C-EC-4B-7F-79-3A */
static u8_t desmac[6] = {0x8c, 0xec, 0x4b, 0x7f, 0x79, 0x3a};
// static u8_t desmac[6] = { 0xF4, 0x8E, 0x38, 0x89, 0x4C, 0xD8};

//static u8_t srcmac[6] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
static u8_t srcmac[6] = {0xE0, 0x78, 0xA3, 0x40, 0x50, 0x60};

u8_t create_link_packet(struct pbuf *p)
{
  u8_t ret;
  u16_t crc;

  /* add ver */
  ret = pbuf_add_header(p, 2);
  if (0 != ret)
  {
    printf("add ver ret %d!\r\n", ret);
    return ret;
  }
  /* IP */
  *(u16_t *)p->payload = lwip_htons(0x0800);

  /* add src mac */
  ret = pbuf_add_header(p, 6);
  if (0 != ret)
  {
    return ret;
  }

  *((u8_t *)p->payload) = srcmac[0];
  *((u8_t *)((u8_t *)p->payload + 1)) = srcmac[1];
  //printf("Network.c +94 ! \r\n"); //hcx_add_fuck
  //*((u32_t *)((u8_t *)p->payload + 2)) = lwip_htonl(LWIP_MAKEU32(srcmac[2],srcmac[3],srcmac[4],srcmac[5]));
  //printf("Network.c +96 ! \r\n");

  /* add des mac */
  ret = pbuf_add_header(p, 6);
  if (0 != ret)
  {
    printf("add des mac %d!\r\n", ret);
    return ret;
  }
  *((u8_t *)p->payload) = desmac[0];
  *((u8_t *)((u8_t *)p->payload + 1)) = desmac[1];
  *((u32_t *)((u8_t *)p->payload + 2)) = lwip_htonl(LWIP_MAKEU32(desmac[2], desmac[3], desmac[4], desmac[5]));

  return ret;
}

u8_t eth_send_if(char *data, unsigned int len);
// uint8_t h_emac_send(char *data, unsigned int FrameLength);
// extern uint8_t emac_send(char *data, unsigned int FrameLength);

static err_t
default_netif_output(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr)
{
  output_ctr++;
  // printf("0 default_netif_output fun ! \r\n");
  create_link_packet(p);
  // printf("1 default_netif_output fun ! \r\n");

#if 0
	eth_send_if(p->payload,p->len);
#else
  emac_send(p->payload, p->len);
#endif

  return ERR_OK;
}

static err_t
default_netif_linkoutput(struct netif *netif, struct pbuf *p)
{
  linkoutput_ctr++;
  printf("1 default_netif_linkoutput fun! \r\n");
  // eth_send_if(p->payload,p->len);

  // emac_send(p->payload, p->len);

  return ERR_OK;
}

static err_t
default_netif_init(struct netif *netif)
{
  netif->output = default_netif_output;
  netif->linkoutput = default_netif_linkoutput;
  netif->mtu = 1500;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
  netif->hwaddr_len = 6;
  return ERR_OK;
}

static void
default_netif_add(void)
{
  struct netif *n;

  IP4_ADDR(&test_ipaddr1, 10, 8, 9, 88);
  IP4_ADDR(&test_netmask1, 255, 255, 255, 0);
  IP4_ADDR(&test_gw1, 10, 8, 9, 1);
  n = netif_add(&test_netif1, &test_ipaddr1, &test_netmask1,
                &test_gw1, NULL, default_netif_init, NULL);
  if (NULL == n)
  {
    printf("netif_add fail!\r\n");
  }
  netif_set_default(&test_netif1);
  netif_set_up(&test_netif1);
}

void network_init()
{
  lwip_init();
  default_netif_add();

  // default_netif_remove();
}

static struct pbuf *udp_create_pbuf(u16_t length, u8_t *data)
{
  err_t err;
  struct pbuf *p;

  p = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);
  if (p == NULL)
  {
    return NULL;
  }

  err = pbuf_take(p, data, length);
  if (0 != err)
  {
    printf("pbuf_take err %d!", err);
  }

  return p;
}

/* recv */
static void test_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                      const ip_addr_t *addr, u16_t port)
{
  struct test_udp_rxdata *ctr = (struct test_udp_rxdata *)arg;
  ctr->rx_cnt++;
  ctr->rx_bytes += p->tot_len;

  printf("recv check!\n");

  if (p != NULL)
  {
    pbuf_free(p);
  }
}

extern UDPContext gUdpContext;
uint32_t udpInit()
{
  err_t err;
  const u16_t port = 1234; // 0x1235;
  struct test_udp_rxdata ctr1;

  network_init();

  send_pcb = udp_new();
  if (NULL == send_pcb)
  {
    printf("pcb1 err null!\n");
    return RTPERR;
  }

  ip_set_option(send_pcb, SOF_REUSEADDR);

  err = udp_bind(send_pcb, &test_netif1.ip_addr, port);
  if (0 != err)
  {
    printf("udp_bind err %d!\n", err);
    return RTPERR;
  }
  memset(&ctr1, 0, sizeof(ctr1));
  ctr1.pcb = send_pcb;

  IP4_ADDR(&send_pcb->remote_ip, gUdpContext.ip0, gUdpContext.ip1, gUdpContext.ip2, gUdpContext.ip3);
  send_pcb->remote_port = gUdpContext.dstPort;

  printf("udpInit local_port %d\r\n", send_pcb->local_port);
  udp_connect(send_pcb, &send_pcb->remote_ip, send_pcb->remote_port);

  udp_recv(send_pcb, test_recv, &ctr1);

  return 0;
}

static u32_t sendto(u32_t s, const void *msg, u32_t len, u32_t flags, const struct sockaddr *to, u32_t tolen)
{
  err_t err;
  struct pbuf *q;
  UDPContext *udp = (UDPContext *)to;

  q = udp_create_pbuf(len, (u8_t *)msg);
  if (NULL == q)
  {
    printf("test_udp_create_test_packet err %d!\r\n", err);
  }

  // printf("local_port %d, remote_port %d, len %d\r\n", send_pcb->local_port, send_pcb->remote_port,len);

  err = udp_send(send_pcb, q);
  if (0 != err)
  {
    printf("udp_send err %d!\r\n", err);
    return 0;
  }

  (void)pbuf_free(q);

  return len;
}

uint32_t udpSend(const UDPContext *udp, const uint8_t *data, uint32_t len)
{
  #if 0
  len = sizeof("abcdefghijklmnopqrst");
  memcpy((uint8_t *)data, "abcdefghijklmnopqrst", len);

  printf("udpSend len : %d buffer=\r\n", len);
  for (int i = 0; i < len; i++)
  {
    printf("0x%x, ", data[i]);
  }
  printf("\r\n");
  #endif

  ssize_t num = sendto(udp->socket, data, len, 0, (struct sockaddr *)&udp->servAddr, sizeof(udp->servAddr));
  if (num != len)
  {
    printf("%s sendto err. %d %d\n", __FUNCTION__, (uint32_t)num, len);
    return RTPERR;
  }

  return len;
}

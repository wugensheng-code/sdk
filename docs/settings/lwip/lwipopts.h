#define ARP_TABLE_SIZE                      #{{arp_table_size}}
#define ICMP_DEBUG                          #{{icmp_debug}}
#define IGMP_DEBUG                          #{{igmp_debug}}
#define IP_DEBUG                            #{{ip_debug}}
#define LWIP_DBG_TYPES_ON                   #{{lwip_dbg_types_on}}
#define NETIF_DEBUG                         #{{netif_debug}}
#define PBUF_DEBUG                          #{{pbuf_debug}}
#define SYS_DEBUG                           #{{sys_debug}}
#define TCP_DEBUG                           #{{tcp_debug}}
#define UDP_DEBUG                           #{{udp_debug}}


#define DHCP_DOES_APR_CHECK                 #{{dhcp_does_apr_check}}
#define LWIP_DHCP                           #{{lwip_dhcp}}
#define ICMP_TTL                            #{{icmp_ttl}}
#define LWIP_IGMP                           #{{lwip_igmp}}

#define IP_DEFAULT_TTL                      #{{ip_default_ttl}}
#define IP_FORWARD                          #{{ip_forward}}
#define IP_FRAG                             #{{ip_frag}}
#define IP_OPTIONS_ALLOWED                  #{{ip_options_allowed}}
#define IP_REASS_MAX_PBUFS                  #{{ip_reass_max_pbufs}}

#define IP_REASSEMBLY                       #{{ip_reassembly}}


#define MEM_SIZE                            #{{mem_size}}
#define MEMP_NUM_PBUF                       #{{memp_num_pbuf}}
#define MEMP_NUM_SYS_TIMEOUT                #{{memp_num_sys_timeout}}
#define MEMP_NUM_TCP_PCB                    #{{memp_num_tcp_pcb}}

#define MEMP_NUM_TCP_PCB_LISTEN             #{{memp_num_tcp_pcb_listen}}
#define MEMP_NUM_TCP_SEG                    #{{memp_num_tcp_seg}}
#define MEMP_NUM_UDP_PCB                    #{{memp_num_udp_pcb}}
#define MEMP_NUM_API_MSG                    #{{memp_num_api_msg}}

#define MEMP_NUM_NETBUF                     #{{memp_num_netbuf}}
#define MEMP_NUM_NETCONN                    #{{memp_num_netconn}}
#define MEMP_NUM_TCPIP_MSG_API              #{{memp_num_tcpip_msg_api}}



#define DEFAULT_TCP_RECVMBX_SIZE            #{{default_tcp_recvmbox_size}}
#define DEFAULT_UDP_RECVMBX_SIZE            #{{default_udp_recvmbox_size}}
#define LWIP_TCPIP_CORE_LOCKING_INPUT       #{{lwip_tcpip_core_locking_input}}
#define TCPIP_MBOX_SIZE                     #{{tcpip_mbox_size}}
#define PBUF_LINK_HLEN                      #{{pbuf_link_hlen}}

#define PBUF_POOL_BUFSIZE                   #{{pbuf_pool_bufsize}}
#define PBUF_POOL_SIZE                      #{{pbuf_pool_size}}

#define LWIP_STATS                          #{{lwip_stats}}

#define LWIP_TCP                            #{{lwip_tcp}}
#define TCP_MAXRTX                          #{{tcp_maxrtx}}
#define TCP_MSS                             #{{tcp_mss}}
#define TCP_QUEUE_OOSEQ                     #{{tcp_queue_ooseq}}
#define TCP_SND_BUF                         #{{tcp_snd_buf}}

#define TCP_SYNMAXRTX                       #{{tcp_synmaxrtx}}
#define TCP_TTL                             #{{tcp_ttl}}
#define TCP_WND                             #{{tcp_wnd}}

#define LWIP_UDP                            #{{lwip_udp}}
#define UDP_TTL                             #{{udp_ttl}}
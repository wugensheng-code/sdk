
/* constants_dep.h */

#ifndef CONSTANTS_DEP_H
#define CONSTANTS_DEP_H

#include "al_gbe_ethernetif.h"
#include "al_gbe_hal.h"

#include "lwipopts.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/snmp.h"

/**
 *\file
 * \brief Plateform-dependent constants definition
 *
 * This header defines all includes and constants which are platform-dependent
 *
 */

/* platform dependent */


typedef enum {FALSE = 0, TRUE} Boolean;
typedef signed char Integer8;
typedef unsigned char UInteger8;
typedef signed short Integer16;
typedef unsigned short UInteger16;
typedef signed int Integer32;
typedef unsigned int UInteger32;
typedef char Octet;

#define IF_NAMESIZE             2

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN         16
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef stdout
#define stdout 1
#endif

#define IFACE_NAME_LENGTH         IF_NAMESIZE
#define NET_ADDRESS_LENGTH        INET_ADDRSTRLEN

#define IFCONF_LENGTH 10

#if BYTE_ORDER == LITTLE_ENDIAN
#define PTPD_LSBF
#elif BYTE_ORDER == BIG_ENDIAN
#define PTPD_MSBF
#endif

/* pow2ms(a) = round(pow(2,a)*1000) */

#define pow2ms(a) (((a)>0) ? (1000 << (a)) : (1000 >>(-(a))))

#define ADJ_FREQ_MAX  1024000

/* UDP/IPv4 dependent */

#define SUBDOMAIN_ADDRESS_LENGTH  4
#define PORT_ADDRESS_LENGTH       2
#define PTP_UUID_LENGTH     NETIF_MAX_HWADDR_LEN
#define CLOCK_IDENTITY_LENGTH   8
#define FLAG_FIELD_LENGTH    2

#define PACKET_SIZE  300 /* ptpdv1 value kept because of use of TLV... */

#define PTP_EVENT_PORT    319
#define PTP_GENERAL_PORT  320

#define DEFAULT_PTP_DOMAIN_ADDRESS  "224.0.1.129"
#define PEER_PTP_DOMAIN_ADDRESS     "224.0.0.107"

#define DEFAULT_PTP_DOMAIN_ADDRESS     "224.0.1.129"
#define ALTERNATE_PTP_DOMAIN1_ADDRESS  "224.0.1.130"
#define ALTERNATE_PTP_DOMAIN2_ADDRESS  "224.0.1.131"
#define ALTERNATE_PTP_DOMAIN3_ADDRESS  "224.0.1.132"

#define MM_STARTING_BOUNDARY_HOPS  0x7fff

/* Must be a power of 2 */
#define PBUF_QUEUE_SIZE 64
#define PBUF_QUEUE_MASK (PBUF_QUEUE_SIZE - 1)

/* others */

#define SCREEN_BUFSZ  128
#define SCREEN_MAXSZ  80

#endif /* CONSTANTS_DEP_H_*/


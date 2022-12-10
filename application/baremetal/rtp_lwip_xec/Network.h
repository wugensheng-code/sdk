//
// Modefy by Jiadong Xiao on 2021/6/20.
//

#ifndef RTPSERVER_NETWORK_H
#define RTPSERVER_NETWORK_H

#include "io.h"
#include <stdint.h>

#define u_char unsigned char
#define u_short unsigned short
#define u_long unsigned long

//jordan
struct sockaddr
{
    unsigned short sa_family;
    char sa_data[14];
};

struct in_addr 
{
        union {
        struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { u_short s_w1,s_w2; } S_un_w;
        u_long S_addr;
    } S_un;
  #define s_addr S_un.S_addr
};

struct sockaddr_in
{
    short int sin_family;
    unsigned short int sin_port;
    struct in_addr sin_addr; 
    unsigned char sin_zero[8]; 
};

typedef struct{
    uint8_t ip0;
	  uint8_t ip1;
	  uint8_t ip2;
	  uint8_t ip3;
    uint16_t dstPort;
    struct sockaddr_in servAddr;
    int socket;
}UDPContext;

/* init UDP */
uint32_t udpInit(void);

/* send UDP packet */
uint32_t udpSend(const UDPContext *udp, const uint8_t *data, uint32_t len);

/* send RTP packet */
void RTPS_tream_loop(void);

#endif //RTPSERVER_NETWORK_H


#ifndef _PACKETINFO_HH_
#define _PACKETINFO_HH_

#include "headers.h"


typedef union Transport_t
{
    Tcp_t tcp;
    Udp_t udp;
} Transport;

class PacketInfo
{
    private:
        bool _isTcp(){return ip.protocol == 6;};
        bool _isUdp(){return ip.protocol == 17;}

    public:
        struct timeval time;
        Ethernet_t eth;
        Ipv4_t ip;
        Transport_t trans;
        enum TransMode { TCP = 0, UDP = 1} mode;
    public:
        PacketInfo() = default;
        PacketInfo(const u_char *pkt, const struct timeval &v);
        PacketInfo(PacketInfo &&) = default;
        PacketInfo(const PacketInfo &) = default;
        //PacketInfo(Ethernet_t *, Ipv4_t *, Transport_t *);

};


/* reserved for fast memory copy */
void *memcpy(void *dest, const void *src, size_t n);
#endif

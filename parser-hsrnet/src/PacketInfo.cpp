#include <stdexcept>
#include "PacketInfo.h"

PacketInfo::PacketInfo(const u_char *pkt)
{
    memcpy(&eth, pkt, sizeof(Ethernet_t));
    pkt += sizeof(Ethernet_t);
    if(eth.type != IPV4_T)
        throw std::runtime_error("Invalid packet type! Expect IPV4!");
    
    memcpy(&ip, pkt, sizeof(Ipv4_t));
    pkt += sizeof(Ipv4_t);
    auto isT = _isTcp(), isU = _isUdp();

    if(isT)
    {
        memcpy(&trans.tcp, pkt, sizeof(Tcp_t));
        mode = TCP;
    }
    else if (isU)
    {
        memcpy(&trans.udp, pkt, sizeof(Udp_t));
        mode = UDP;
    }
    else 
        throw std::runtime_error("Invalid packet type! Expect TCP or UDP!");
}

#include <stdexcept>
#include "PacketInfo.h"

PacketInfo::PacketInfo(const u_char *pkt, const struct timeval &v)
    :time(v) 
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

        pkt += sizeof(Tcp_t);
        int size = trans.tcp.doff * 4;
        int addi = size - 20;
        if(addi) memcpy(options, pkt, 
                 std::min((uint64_t)addi, sizeof options));
        this->payload = ntohl(ip.tot_len) - ip.ihl * 4 - trans.tcp.doff * 4;
    }
    else if (isU)
    {
        memcpy(&trans.udp, pkt, sizeof(Udp_t));
        mode = UDP;

        this->payload = ntohl(ip.tot_len) - ip.ihl * 4 - sizeof(Udp_t);
    }
    else 
        throw std::runtime_error("Invalid packet type! Expect TCP or UDP!");
    
    
}

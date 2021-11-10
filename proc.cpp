#include "icmp.h"

void ICMP::proc(char *buf, ssize_t len, msghdr *msg, timeval *tvrecv){
    int hlen1, icmplen;
    double rtt;
    ip *ip;
    header *icmp_hdr;
    timeval *tvsend;
    ip = (struct ip*)buf;
    hlen1 = ip->ip_hl << 2;
    if(ip->ip_p != IPPROTO_ICMP)
        return;
    icmp_hdr = (header*)(buf+hlen1);
    if((icmplen = len -hlen1) < 8)
        return;
    if(icmp_hdr->type == ICMP_ECHOREPLY){
        if(icmp_hdr->id != pid)
            return;
        if(icmplen < 16)
            return;
        tvsend = (timeval*) icmp_hdr->data;
        tv_sub(tvrecv, tvsend);
        rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec/1000.0;
        std::setprecision(3);
        std::cout << icmplen << " bytes from " << sarecv->sa_data << ": seq=";
        std::cout << icmp_hdr->seq << ", ttl=" << ip->ip_ttl << ", rtt=";
        std::cout << std::fixed << rtt << " ms\n";
    }
    else if(verbose){
        std::cout << icmplen << " bytes from " << sarecv->sa_data << ", type = ";
        std::cout << icmp_hdr->type << ", code = " << icmp_hdr->code << std::endl;
    }
}
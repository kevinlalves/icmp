#include "icmp.h"

void ICMP::proc(char *buf, ssize_t len, Time *tvrecv){
    int icmplen, lenhdr_ip;
    std::chrono::duration<double,std::milli> rtt;
    header *icmp_hdr;
    Time tvsend;
    ip *ip;
    ip = (struct ip*)buf;
    lenhdr_ip = ip->ip_hl << 2;
    icmp_hdr = (header*)(buf+lenhdr_ip);
    if(ip->ip_p != IPPROTO_ICMP)
        return;
    if((icmplen = len-lenhdr_ip) < 8)
        return;
    if(icmp_hdr->id == pid){
        tvsend = icmp_hdr->data;
        rtt = *tvrecv - tvsend;
        std::cout << std::setprecision(3);
        std::cout << icmplen << " bytes from " << canonname << ": seq=";
        std::cout << icmp_hdr->seq << ", ttl=" << int(ip->ip_ttl) << ", time=";
        std::cout << std::fixed << rtt.count() << " ms\n";
    }
}
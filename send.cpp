#include "icmp.h"


int ICMP::checksum(uint16_t *addr, int len){
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    while(len > 1){
        sum += *w++;
        len -= 2;
    }
    if(len == 1){
        *(unsigned char*)(&answer) = *(unsigned char*) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

void ICMP::send(){

    int len;
    while(1){
        hdr.type = ICMP_ECHO;
        hdr.code = 0;
        hdr.id = pid;
        hdr.seq++;

        len = 8 + datalen;
        hdr.cksum = 0;
        hdr.cksum = checksum((uint16_t *)&hdr, len);

        hdr.data = Clock::now();
        sendto(sockfd, &hdr, len, 0, sasend, salen);
        sleep(1);
    }
}
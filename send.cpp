#include "icmp.h"
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>

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

        memset(hdr.data, 0xa5, datalen);
        gettimeofday((struct timeval *)hdr.data, NULL);

        len = 8 + datalen;
        hdr.cksum = checksum((uint16_t *)&hdr, len);

        sendto(sockfd, sendbuf, len, 0, sasend, salen);
        sleep(1);
    }
}
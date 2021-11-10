#include "icmp.h"
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>

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
        hdr.cksum = chekcsum();

        sendto(sockfd, sendbuf, len, 0, sasend, salen);
        sleep(1);
    }
}
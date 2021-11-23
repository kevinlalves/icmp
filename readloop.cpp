#include "icmp.h"

void ICMP::readloop(){

    char recvbuf[BUFFSIZE];
    ssize_t n;
    Time *tval = new Time;

    while(1){
        n = recvfrom(sockfd, recvbuf, BUFFSIZE, 0, sasend, &salen);
        if(n < 0){
            if(errno == EINTR)
                continue;
            else
                perror("recvmsg error");
                exit(1);
        }

        *tval = Clock::now();
        proc(recvbuf, n, tval);
    }

}
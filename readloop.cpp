#include "icmp.h"

void ICMP::readloop(){
    int size;
    char recvbuf[BUFFSIZE];
    char controlbuf[BUFFSIZE];
    msghdr msg;
    iovec iov;
    ssize_t n;
    timeval tval;

    sockfd = socket(sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    setuid(getuid());

    size = 60 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    iov.iov_base = recvbuf;
    iov.iov_len = sizeof(recvbuf);
    msg.msg_name = sarecv;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;
    while(1){
        msg.msg_namelen = salen;
        msg.msg_controllen;
        n = recvmsg(sockfd, &msg, 0);
        if(n < 0){
            if(errno == EINTR)
                continue;
            else
                perror("recvmsg error");
                exit(1);
        }

        gettimeofday(&tval, NULL);
        proc(recvbuf, n, &msg, &tval);
    }

}
// icmp.h -- interface for ICMP echo communication between server-client

#ifndef _ICMP20_H_
#define _ICMP20_H_

#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

const int BUFFSIZE = 1500;

class ICMP{
    public:
        int echo(char *host);
        ICMP();
    private:
        void send();
        void proc(char *buf, ssize_t len, msghdr *msg, timeval *tvrecv);
        void readloop();
        void tv_sub(timeval *out, timeval *in);
        addrinfo *host_serv(const char *hostname, const char *service, int family, int socktype);
        int checksum(uint16_t *addr, int len);
        struct header{
            unsigned short type;
            unsigned short code;
            unsigned short id;
            unsigned short seq;
            unsigned short cksum;
            char *data;
        } hdr;
        int datalen;
        char sendbuf[BUFFSIZE];
        char *host;
        pid_t pid;
        int sockfd;
        int verbose;
        sockaddr *sasend;
        sockaddr *sarecv;
        socklen_t salen;
        int protocol;
};


#endif
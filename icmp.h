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
#include <chrono>
#include <ctime>
#define Clock std::chrono::system_clock
#define Time std::chrono::time_point<Clock>

//only for debug purposes
#define debug(x) std::cout << x << std::endl; 
//

const int BUFFSIZE = 1500;

class ICMP{
    public:
        int echo(char *host);
        ICMP();
    private:
        void send();
        void proc(char *buf, ssize_t len, Time *tvrecv);
        void readloop();
        addrinfo *host_serv(const char *hostname, const char *service, int family, int socktype);
        int checksum(uint16_t *addr, int len);
        struct header{
            uint8_t type;
            uint8_t code;
            uint16_t cksum;
            uint16_t id;
            uint16_t seq;
            Time data;
        } hdr;
        int datalen;
        char sendbuf[BUFFSIZE];
        char *host;
        pid_t child_pid, pid;
        int sockfd;
        sockaddr *sasend;
        socklen_t salen;
        int protocol;
        std::string canonname;
};


#endif
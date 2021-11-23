#include "icmp.h"


// module to interact with client program
int ICMP::echo(char *host){
    addrinfo *ai;
    ai = host_serv(host, NULL, 0, SOCK_RAW);
    if(ai == NULL){ //error: not a valid address info
        std::cout << "Error: Invalid host name\n";
        exit(1);
    }
    if(ai->ai_family != AF_INET){ //error: not  a IPv4 packet
        std::cout << "Error: Only support IPv4 addresses\n";
        exit(1);
    } 
    sasend = ai->ai_addr;
    salen = ai->ai_addrlen;
    canonname = ai->ai_canonname;

    sockfd = socket(sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    setuid(getuid());

    int size = 60 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    if((child_pid = fork()))
        readloop();
    else     
        send();
    return 0;
}

ICMP::ICMP(){
    hdr.seq = 0;
    datalen = sizeof(Time);
    pid = getpid()&0xffff;
}

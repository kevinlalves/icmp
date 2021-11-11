#include "icmp.h"


// module to interact with client program
int ICMP::echo(char *host){
    addrinfo *ai;
    pid = getpid() & 0xffff;
    if(fork())
        send();
    else{
        ai = host_serv(host, NULL, 0, 0);
        if(ai->ai_family != AF_INET)
            return -1; //error: not  a IPv4 packet
        sasend = ai->ai_addr;
        sarecv = new sockaddr;
        salen = ai->ai_addrlen;
        readloop();
    }
    return 0;
}

ICMP::ICMP(){
    hdr.seq = 0;
}
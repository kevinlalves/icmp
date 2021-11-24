#include "icmp.h"
#include <chrono>
#include <netdb.h>
#include <string.h>
#include <iomanip>
#include <signal.h>
#include <netinet/ip.h>
#define Clock chrono::high_resolution_clock 
#define Time chrono::time_point<Clock>

using namespace std;

int echo(char *host);
void send();
void processing(char *buf, ssize_t len, Time tvrecv);
void readloop();
addrinfo *host_serv(const char *hostname, const char *service, int family, int socktype);

const int BUFFSIZE = 1500;
int sockfd;
sockaddr *sa;
socklen_t salen;
pid_t pid;
string canonname;

int main(int argc, char **argv){
    char *host;
    if(argc != 2){
        std::cout << "Usage: ping <hostname>\n";
        exit(1);
    } 
    //error: wrong hostname info
    host=argv[1];
    echo(host);
    return 0;
}


addrinfo *host_serv(const char *hostname, const char *service, int family, int socktype){
    int n;
    addrinfo hints, *res;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    if((n = getaddrinfo(hostname, service, &hints, &res)) != 0)
        return NULL;
    return res; 
}


// open socket and start communication with host
int echo(char *host){
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
    sa= ai->ai_addr;
    salen = ai->ai_addrlen;
    canonname = ai->ai_canonname;

    sockfd = socket(sa->sa_family, SOCK_RAW, IPPROTO_ICMP);
    setuid(getuid());

    int size = 60 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));


    if(pid = fork())
        readloop();
    else     
        send();
    return 0;
}

void send(){
    ICMP pkt;
    Time tval;
    std::vector<uint8_t> byte_array;
    int len;
    long time_epoch;
    while(1){
        pkt.increment_seq();
        tval = Clock::now();
        time_epoch = tval.time_since_epoch().count();
        pkt.set_payload(time_epoch);
        byte_array = pkt.encode();
        sendto(sockfd, byte_array.data(), byte_array.size(), 0, sa, salen);
        sleep(1);
    }
}

void readloop(){
    char recvbuf[BUFFSIZE];
    Time tvrecv;
    ssize_t n;
    while(1){
        n = recvfrom(sockfd, recvbuf, BUFFSIZE, 0, sa, &salen);
        tvrecv = Clock::now();
        if(n < 0){
            if(errno == EINTR)
                continue;
            else
                perror("recvmsg error");
                exit(1);
        }
        processing(recvbuf, n, tvrecv);
    }

}




void processing(char *buf, ssize_t len, Time tvrecv){
    int len_hdr_ip;
    ICMP pkt;
    double rtt;
    int64_t t_epoch_recv = tvrecv.time_since_epoch().count();
    int64_t t_epoch_send = 0, term;
    ip *ip;
    ip = (struct ip*)buf;
    len_hdr_ip = ip->ip_hl << 2;
    if(ip->ip_p != IPPROTO_ICMP)
        return;
    std::vector<uint8_t> byte_array((uint8_t*)(buf+len_hdr_ip), (uint8_t*)(buf+len));
    if(pkt.decode(byte_array) != 0)
        return;
    if(pkt.check_id(pid)){
        std::vector<uint8_t> data((uint8_t*)(buf+len_hdr_ip+8),(uint8_t*)(buf+len));
        int i = 0;
        for (auto it = data.begin(); it != data.end(); it++, i++){
            term = *it;
            t_epoch_send += term<<(8*i);
        }
        rtt = (t_epoch_recv-t_epoch_send)/1e6;
        std::cout << len-len_hdr_ip << " bytes from " << canonname << ": "; 
        pkt.to_string();
        cout << setprecision(4) << fixed;
        cout << ", ttl=" << int(ip->ip_ttl) << ", time=" << rtt  << " ms" << endl;
    }
}



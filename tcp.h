// ping.h -- interface for communication between client and server TCP

#ifndef __tcp_h_
#define __tcp_h_

#include "icmp.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>

#include <chrono>
#include <iomanip>
#include <string>
#include <vector>

#define Clock std::chrono::high_resolution_clock 
#define Time std::chrono::time_point<Clock>

const int kBuffSize = 1500;
const int kMaxQueue = 5;

class Tcp {
  public:
    void StartClient(char *host, char *port);
    void StartServer(char *port);
  private:
    int sockfd; //number referencing open socket
    int connfd;
    char *port_;
    char *host_;
    sockaddr_in servaddr; //formated server address
    Time tvrecv;
    char buff[kBuffSize]; // buffer used in all communications
    void OpenSock(sa_family_t family, in_addr_t host_addr, uint16_t port);
    void ClientSock();
    void ServerSock();
    void ClientChatting();
    void ServerChatting();
};

#endif
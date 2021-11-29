// ping.h -- interface for communication between client and server ICMP

#ifndef __ping_h_
#define __ping_h_

#include "icmp.h"

#include <netinet/ip.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>

#include <chrono>
#include <iomanip>
#include <vector>

#define Clock std::chrono::high_resolution_clock 
#define Time std::chrono::time_point<Clock>

const int kBuffSize = 1500;

class Ping {
  public:
    void StartClient(char *host, char *port);
    void StartServer(char *host, char *port);
    void ClientStatistics();
    void ServerStatistics();
    struct sigaction old_action; // responsible to kill child process
  private:
    int sockfd; //number referencing open socket
    sockaddr *sa; //formated host address
    socklen_t salen;
    pid_t pid; // id of process(child) sending packets
    std::string canonname; // string name of the host's address
    Time tvrecv;
    char buf[kBuffSize];

    // data for statistics
    uint16_t last_seq = 0; // sentinel value, no packets received
    int received_packets = 0;
    double total_time = 0;
    double min_rtt = 1e8, max_rtt = -1.0; //sentinel values 

    void OpenSock();
    int HostServ(const char *hostname, const char *port, int family, int socktype);
    void Processing(ssize_t len, uint8_t type);
    void Send(uint8_t type);
    void Read(uint8_t type);
};

#endif
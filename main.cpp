#include "icmp.h"

#include <netdb.h>
#include <netinet/ip.h>
#include <string.h>
#include <signal.h>

#include <chrono>
#include <iomanip>

#define Clock std::chrono::high_resolution_clock 
#define Time std::chrono::time_point<Clock>

using std::cout;
using std::endl;

int Echo(char *host);
void Send();
void Processing(char *buf, ssize_t len, Time tvrecv);
void ReadLoop();
addrinfo *HostServ(const char *hostname, const char *service, int family, int socktype);
void SigintHandler(int sig_num);

const int kBuffSize = 1500;
int sockfd; //number referencing open socket
sockaddr *sa; //formated host address
socklen_t salen;
pid_t pid; // id of process(child) sending packets
std::string canonname; // string name of the host's address
struct sigaction old_action; // responsible to kill child process

int main(int argc, char **argv) {
  char *host;
  if(argc != 2){
    std::cout << "Usage: ping <hostname>\n";
    exit(1);
  } 
  //error: wrong hostname info
  host=argv[1];
  Echo(host);
  return 0;
}

addrinfo *HostServ(const char *hostname, const char *service, int family, int socktype) {
  int n;
  addrinfo hints, *res;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_flags = AI_CANONNAME;
  hints.ai_family = family;
  hints.ai_socktype = socktype;
  if ((n = getaddrinfo(hostname, service, &hints, &res)) != 0)
    return NULL;
  return res; 
}

// open socket and start communication with host
int Echo(char *host) {
  addrinfo *ai;
  ai = HostServ(host, NULL, 0, SOCK_RAW);
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

  if (pid = fork()) {
    ReadLoop();
  } else {     
    Send();
  }
  return 0;
}

void Send() {
  Icmp packet;
  Time tval;
  std::vector<uint8_t> byte_array;
  int len;
  long time_epoch;
  while (1) {
    packet.IncrementSeq();
    tval = Clock::now();
    time_epoch = tval.time_since_epoch().count();
    packet.SetPayload(time_epoch);
    byte_array = packet.Encode();
    sendto(sockfd, byte_array.data(), byte_array.size(), 0, sa, salen);
    sleep(1);
  }
}

void ReadLoop() {
  struct sigaction action;
  char recvbuf[kBuffSize];
  Time tvrecv;
  ssize_t n;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &SigintHandler;
  sigaction(SIGINT, &action, &old_action);
  while (1) {
    n = recvfrom(sockfd, recvbuf, kBuffSize, 0, sa, &salen);
    tvrecv = Clock::now();
    if (n < 0) {
      if(errno == EINTR) {
        continue;
      } else
        perror("recvmsg error");
        exit(1);
      }
      Processing(recvbuf, n, tvrecv);
    }

}

void Processing(char *buf, ssize_t len, Time tvrecv) {
  int len_hdr_ip;
  Icmp packet;
  double rtt;
  int64_t t_epoch_recv = tvrecv.time_since_epoch().count();
  int64_t t_epoch_send = 0, term;
  ip *ip;
  ip = (struct ip*)buf;
  len_hdr_ip = ip->ip_hl << 2;
  if (ip->ip_p != IPPROTO_ICMP)
    return;
  std::vector<uint8_t> byte_array((uint8_t*)(buf+len_hdr_ip), (uint8_t*)(buf+len));
  if (packet.Decode(byte_array) != 0)
    return;
  if (packet.CheckId(pid)) {
    std::vector<uint8_t> data((uint8_t*)(buf+len_hdr_ip+8),(uint8_t*)(buf+len));
    int i = 0;
    for (auto it = data.begin(); it != data.end(); it++, i++) {
      term = *it;
      t_epoch_send += term<<(8*i);
    }
    rtt = (t_epoch_recv-t_epoch_send)/1e6;
    cout << len-len_hdr_ip << " bytes from " << canonname << ": "; 
    packet.ToString();
    cout << std::setprecision(4) << std::fixed;
    cout << ", ttl=" << int(ip->ip_ttl) << ", time=" << rtt  << " ms" << endl;
    }
}

void SigintHandler(int sig_num) {
  sigaction(SIGINT, &old_action, NULL);
  cout << endl << "--- " << canonname << " ping statistics ---" << endl;
  kill(pid,SIGKILL);
  kill(0,SIGINT);
}

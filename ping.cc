#include "ping.h"
#include "icmp.h"

int Ping::HostServ(const char *hostname, const char *port, int family, int socktype) {
  int n;
  addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_CANONNAME;
  hints.ai_family = family;
  hints.ai_socktype = socktype;
  if ((n = getaddrinfo(hostname, port, &hints, &res)) != 0)
    return 1;

  sa= res->ai_addr;
  salen = res->ai_addrlen;
  canonname = res->ai_canonname;
  return 0;
}

void Ping::OpenSock() {
    sockfd = socket(sa->sa_family, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1)
      std::cout << "Remember to run as superuser." << std::endl;
    setuid(getuid());

    int size = 60 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}

void Ping::ClientSigHandler(int sig_num) {
  std::cout << std::endl << "--- " << client.canonname;
  std::cout << " ping statistics ---" << std::endl;
  std::cout << client.last_seq << " transmitted packets, " << client.received_packets; 
  std::cout << " received, ";
  if (client.last_seq > 0) {
    double lost_packet_percent = 100.0*(client.last_seq-client.received_packets)/client.last_seq;
    std::cout << int(lost_packet_percent) << "%" << " packages lost, ";
  }
  std::cout << "time " << client.total_time << " ms" << std::endl;

  if (client.received_packets > 0) {
    std::cout << "rtt min/avg/max = " << client.min_rtt << "/";
    std::cout << client.total_time/client.received_packets;
    std::cout << "/" << client.max_rtt << " ms" << std::endl;
  }

  sigaction(SIGINT, &client.old_action, NULL);
  kill(0, SIGINT);
}


void Ping::ServerSigHandler(int sig_num) {
  std::cout << std::endl << "--- " << server.canonname;
  std::cout << " server statistics ---" << std::endl;
  std::cout << client.received_packets << " received\n";

  sigaction(SIGINT, &server.old_action, NULL);
  kill(0, SIGINT);
}

void Ping::Send(uint8_t type) {
  Icmp packet;
  Time tval;
  std::vector<uint8_t> byte_array;
  int len;
  int64_t time_epoch;

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

void Ping::Processing(ssize_t len, uint8_t type) {
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

  if (packet.CheckCompatibility(type, 0, pid)) {
    received_packets++;
    last_seq = packet.GetSeq();
    std::vector<uint8_t> data((uint8_t*)(buf+len_hdr_ip+8),(uint8_t*)(buf+len));
    int i = 0;
    for (auto it = data.begin(); it != data.end(); it++, i++) {
      term = *it;
      t_epoch_send += term<<(8*i);
    }
    rtt = (t_epoch_recv-t_epoch_send)/1e6;

    total_time += rtt;
    if (rtt < min_rtt)
      min_rtt = rtt;
    if (rtt > max_rtt)
      max_rtt = rtt;

    std::cout << len-len_hdr_ip << " bytes from " << canonname << ": "; 
    packet.ToString();
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << ", ttl=" << int(ip->ip_ttl) << ", time=" << rtt  << " ms" << std::endl;
    }
}

void Ping::Read(uint8_t type) {
  ssize_t n;
  while (1) {
    n = recvfrom(sockfd, buf, kBuffSize, 0, sa, &salen);
    tvrecv = Clock::now();
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else
        perror("recvmsg error");
        exit(1);
      }
      Processing(n, type);
    }

}

void Ping::StartClient(char *host, char *port) {
  HostServ(host, port, 0, SOCK_RAW);
  OpenSock();

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ClientSigHandler;
  sigaction(SIGINT, &action, &old_action);

  if (pid = fork()) {
    Read(kIcmpReply);
  } else {
    Send(kIcmpEcho);
  }
  
}

void Ping::StartServer(char *host, char *port) {
  HostServ(host, port, 0, SOCK_RAW);
  OpenSock();

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ServerSigHandler;
  sigaction(SIGINT, &action, &old_action);

  if (pid = fork()) {
    Read(kIcmpEcho);
  } else {
    Send(kIcmpReply);
  }
}
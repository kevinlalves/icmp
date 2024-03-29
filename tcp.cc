#include "tcp.h"

#include "icmp.h"

void Tcp::OpenSock(sa_family_t family, in_addr_t host_serv, uint16_t port) {
  // socket create and verification
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ == -1) {
    std::cout << "Socket creation failed...\n";
    exit(0);
  } else {
    std::cout << "Socket successfully created...\n";
  }
  bzero(&servaddr_, sizeof(servaddr_));
   
  // assign IP, PORT
  servaddr_.sin_family = family;
  servaddr_.sin_addr.s_addr = host_serv;
  servaddr_.sin_port = htons(port);
}

void Tcp::ServerSock() {
  OpenSock(AF_INET, INADDR_ANY, std::stoi(port_));

  sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  // Binding newly created socket to given IP and verification
  if ((bind(sockfd_, (sockaddr*)&servaddr_, sizeof(servaddr_))) != 0) {
    std::cout << "Socket bind failed...\n";
    exit(0);
  } else {
    std::cout << "Socket successfully binded...\n";
  }
   
  // Now server is ready to listen and verification
  if ((listen(sockfd_, kMaxQueue)) != 0) {
    std::cout << "Listen failed...\n";
    exit(0);
  } else {
    std::cout << "Server listening...\n";
  }
   
  // Accept the data packet from client and verification
  connfd_ = accept(sockfd_, (sockaddr*)&client_addr, &len);
  if (connfd_ < 0) {
    std::cout << "Server accept failed...\n";
    exit(0);
  } else {
    std::cout << "Server accept the client...\n";
  }
}

void Tcp::ClientSock() {
  OpenSock(AF_INET, inet_addr(host_), std::stoi(port_));
  
  // connect the client socket to server socket
  if (connect(sockfd_, (sockaddr*)&servaddr_, sizeof(servaddr_)) != 0) {
    std::cout << "Connection with the server failed...\n";
    exit(0);
  } else {
    std::cout << "Connected to the server...\n";
  }
}

void Tcp::ClientChatting() {
  Icmp packet;
  Time tvrecv;
  int64_t now_to_epoch, sent_to_epoch;
  char buff[kBuffSize]; // buffer used in all communications
  double rtt; // round trip time
  std::vector<uint8_t> byte_array;
  std::vector<uint8_t> data;
  while (1) {
    packet.IncrementSeq();
    tvrecv = Clock::now();
    now_to_epoch = tvrecv.time_since_epoch().count();
    data.clear();
    data.insert(data.end(),(uint8_t*)&now_to_epoch, 
               ((uint8_t*)&now_to_epoch)+sizeof(now_to_epoch));
    packet.SetPayload(data);

    byte_array = packet.Encode();
    write(sockfd_, byte_array.data(), byte_array.size());
    bzero(buff, sizeof(buff));
    read(sockfd_, buff, byte_array.size());

    tvrecv = Clock::now();
    now_to_epoch = tvrecv.time_since_epoch().count();
    sent_to_epoch = packet.DecodeData();
    rtt = 1.0*(now_to_epoch-sent_to_epoch)/1e6;
    std::cout << byte_array.size() << " bytes from (" << host_ << "): ";
    packet.ToString();
    std::cout << ", rtt=" << rtt << "ms" << std::endl;
    sleep(1);
  }
}

void Tcp::ServerChatting() {
  Icmp packet;
  char buff[kBuffSize]; // buffer used in all communications
  while (1) {
    bzero(buff, sizeof(buff));
    read(connfd_, buff, sizeof(buff)) ;
    // get structured packet from bytes
    std::vector<uint8_t> byte_array((uint8_t*)buff, (uint8_t*)(buff+sizeof(buff)));
    if (packet.Decode(byte_array) == 0) {
      std::cout << "Packet: "; 
      packet.ToString();
    } else {
      std::cout << "Corrupted Packet received";
    }
    std::cout << std::endl;
    // send a reply in response to the original echo
    *(uint8_t*)buff = kIcmpReply;
    write(connfd_, buff, sizeof(buff));
  } 
}

void Tcp::StartClient(char *host, char *port) {
  host_ = host;
  port_ = port;
  ClientSock();
  ClientChatting();
  close(sockfd_);
}

void Tcp::StartServer(char *port) {
  port_ = port;
  ServerSock();
  ServerChatting();
  close(connfd_);
  close(sockfd_);
}
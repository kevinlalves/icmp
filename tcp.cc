#include "tcp.h"

#include "icmp.h"

void Tcp::OpenSock(sa_family_t family, in_addr_t host_serv, uint16_t port) {
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = family;
    servaddr.sin_addr.s_addr = host_serv;
    servaddr.sin_port = htons(port);
}

void Tcp::ServerSock() {
  OpenSock(AF_INET, INADDR_ANY, std::stoi(port_));

  sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, kMaxQueue)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (sockaddr*)&client_addr, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
}

void Tcp::ClientSock() {
  OpenSock(AF_INET, inet_addr(host_), std::stoi(port_));
  
  // connect the client socket to server socket
    if (connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
}

void Tcp::ClientChatting() {
  while (1) {
    bzero(buff, sizeof(buff));
    std::cout << "Enter the string : ";
    std::cin.getline(buff, kBuffSize);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    while (read(sockfd, buff, sizeof(buff)) <= 0)
      ;
    std::cout << "From Server : " << buff << std::endl;
    if ((strncmp(buff, "exit", 4)) == 0) {
      printf("Client Exit...\n");
      break;
    }
  }
}

void Tcp::ServerChatting() {
  while (1) {
    bzero(buff, sizeof(buff));
   
    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff)) ;
    // print buffer which contains the client contents
    std::cout << "From client: " << buff << std::endl;
    std::cout << "To client : ";
    bzero(buff, kBuffSize);
    // copy server message in the buffer
    std::cin.getline(buff, kBuffSize);
    // and send that buffer to client
    write(connfd, buff, sizeof(buff));
   
    // if msg contains "Exit" then server exit and chat ended.
    if (strncmp("exit", buff, 4) == 0) {
      printf("Server Exit...\n");
      break;
    }
  } 
}

void Tcp::StartClient(char *host, char *port) {
  host_ = host;
  port_ = port;
  ClientSock();
  ClientChatting();
}

void Tcp::StartServer(char *port) {
  port_ = port;
  ServerSock();
  ServerChatting();
}
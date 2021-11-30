#include "icmp.h"
#include "tcp.h"

int main(int argc, char **argv) {
  Tcp server;
  char *port;
  if(argc != 2){
    std::cout << "Usage: server <port>\n";
    exit(1);
  } //error: wrong hostname info
  port = argv[1];
  server.StartServer(port);
  return 0;
}
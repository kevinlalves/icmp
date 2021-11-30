#include "tcp.h"

int main(int argc, char **argv) {
  Tcp client;
  char *host; 
  char *port;
  if(argc != 3){
    std::cout << "Usage: client <hostname> <port>\n";
    exit(1);
  }  //error: wrong hostname info
  host = argv[1];
  port = argv[2];
  client.StartClient(host, port);
  return 0;
}
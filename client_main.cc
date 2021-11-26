#include "icmp.h"
#include "ping.h"

int main(int argc, char **argv) {
  char *host; 
  char *port;
  if(argc != 3){
    std::cout << "Usage: client <hostname> <port>\n";
    exit(1);
  } 
  //error: wrong hostname info
  host = argv[1];
  port = argv[2];
  Ping::client.StartClient(host, port);
  return 0;
}
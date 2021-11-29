#include "icmp.h"
#include "ping.h"

void ClientSigHandler(int sig_num);
Ping client;

int main(int argc, char **argv) {
  char *host; 
  char *port;
  if(argc != 3){
    std::cout << "Usage: client <hostname> <port>\n";
    exit(1);
  }  //error: wrong hostname info
  host = argv[1];
  port = argv[2];

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ClientSigHandler;
  sigaction(SIGINT, &action, &client.old_action);

  client.StartClient(host, port);
  return 0;
}

void ClientSigHandler(int sig_num) {
  client.ClientStatistics();
  sigaction(SIGINT, &client.old_action, NULL);
  kill(0, SIGINT);
}

#include "icmp.h"
#include "ping.h"

void ServerSigHandler(int sig_num);
Ping server;

int main(int argc, char **argv) {
  char *host;
  char *port;
  if(argc != 3){
    std::cout << "Usage: server <hostname> <port>\n";
    exit(1);
  } //error: wrong hostname info
  host = argv[1];
  port = argv[2];

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ServerSigHandler;
  sigaction(SIGINT, &action, &server.old_action);

  server.StartServer(host, port);
  return 0;
}

void ServerSigHandler(int sig_num) {
  server.ServerStatistics();
  sigaction(SIGINT, &server.old_action, NULL);
  kill(0, SIGINT);
}
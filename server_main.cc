#include "icmp.h"
#include "ping.h"

void ServerSigHandler(int sig_num);
Ping server;

int main(int argc, char **argv) {
  char *port;
  if(argc != 2){
    std::cout << "Usage: server <port>\n";
    exit(1);
  } //error: wrong hostname info
  port = argv[1];

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ServerSigHandler;
  sigaction(SIGINT, &action, &server.old_action);

  server.StartServer(port);
  return 0;
}

void ServerSigHandler(int sig_num) {
  server.ServerStatistics();
  sigaction(SIGINT, &server.old_action, NULL);
  kill(0, SIGINT);
}
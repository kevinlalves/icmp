#include "icmp.h"
#include "tcp.h"

int main(int argc, char **argv) {
<<<<<<< HEAD
=======
  Tcp server;
>>>>>>> 3098477cae95c5dce0d9de70473c828c4ccf2515
  char *port;
  if(argc != 2){
    std::cout << "Usage: server <port>\n";
    exit(1);
  } //error: wrong hostname info
  port = argv[1];
<<<<<<< HEAD

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &ServerSigHandler;
  sigaction(SIGINT, &action, &server.old_action);

=======
>>>>>>> 3098477cae95c5dce0d9de70473c828c4ccf2515
  server.StartServer(port);
  return 0;
}
#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char pp[HANDSHAKE_BUFFER_SIZE];
  char client_return[HANDSHAKE_BUFFER_SIZE];
  printf("Server created wkp\n");
  mkfifo("wkp",0666);

  printf("Server is waiting for client\n");
  int clientu = open ("wkp", O_RDONLY);
  printf("Server is connected to client upstream\n");
  
  read(clientu,pp, sizeof(pp));
  printf("Server has received signal\n");

  int clientd = open(pp,O_WRONLY);
  printf("Downstream pipe created\n");

  remove("wkp");
  printf("Wkp is removed\n");

  printf("Sending signal to client\n");
  write(clientd,ACK,sizeof(ACK));
  printf("Awaiting client response\n");
  read(clientu,client_return,sizeof(client_return));
  printf("Client validation message received\n");

  return clientu;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
   char pp[HANDSHAKE_BUFFER_SIZE];
  char signal[HANDSHAKE_BUFFER_SIZE];
  
  printf("Client created private pipe\n");
  mkfifo(pp, 0666);

  int serveru = open("wkp", O_WRONLY);
  printf("Client connected to server upstream\n");

  write(serveru,pp,sizeof(pp));
  printf("Client signal sent\n");

  int serverd = open(pp,O_RDONLY);
  printf("Client connected to server downstream\n");

  read(serverd,signal, sizeof(signal));
  printf("Received signal from server\n");

  remove(pp);
  printf("Private pipe removed\n");

  write(serveru,ACK,sizeof(ACK));
  printf("Client validation message sent\n");

  *to_server = serveru;
  
  return serverd;
}

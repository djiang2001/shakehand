#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char ups[HANDSHAKE_BUFFER_SIZE];
  char msg[BUFFER_SIZE];
    
  printf("Server created wkp\n");
  mkfifo("wkp", 0666);

  printf("Server waiting for client\n");

  int wkp = open("wkp", O_RDONLY);
  read(wkp,ups, HANDSHAKE_BUFFER_SIZE);
  printf("Message from client: %s\n", ups);
  
  printf("Server removed\n");
  remove("wkp");

  printf("Server asking for validation\n ");
  *to_client = open("pp", O_WRONLY);
  write(*to_client, ACK, sizeof(ACK));

  printf("Server received validation\n");

  read(wkp, msg, BUFFER_SIZE);
  
  return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char ups[HANDSHAKE_BUFFER_SIZE];
    
  printf("Client created private pipe\n");
  mkfifo("pp", 0666);

  printf("Client connected to server\n");
  *to_server = open("wkp", O_WRONLY);
  
  printf("Client sent private pipe message\n");
  write(*to_server, "pp", 6);

  printf("Client confirmed server response\n");

  int pp = open("pp", O_RDONLY);
  read(pp, ups, HANDSHAKE_BUFFER_SIZE);
  printf("Message from server: %s\n", ups);

  remove("pp");
  printf("Client removed private pipe\n");
  
  printf("Client sending validation\n");
  write(*to_server, ACK, sizeof(ACK));
  
  return pp;
}

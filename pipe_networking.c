#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  //remove("WKP");
  printf("Creating well known pipe\n");
    int pipe_f;
    if ((pipe_f = mkfifo("WKP", 0644)) == -1) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }
  printf("Connecting to WKP\n");
    int wkp_fd;
    if(wkp_fd = open("WKP", O_RDONLY) == -1){
      printf("Error %d: %s\n", errno, strerror(errno));
    }
  printf("Connected to WKP\n");

  printf("Waiting for client to respond\n");

  char msg[BUFFER_SIZE];
  read(wkp_fd, msg, BUFFER_SIZE);
  //printf("Client said: %s", msg);

  int pvt_fd;
  pvt_fd = open(msg, O_WRONLY);

    printf("Sending message to client \n" );

    write(pvt_fd,ACK,HANDSHAKE_BUFFER_SIZE);
  //  printf("server sent client ACK message: %s\n",ACK);

    printf("waiting for response from client\n");

    char ack_mes[HANDSHAKE_BUFFER_SIZE];
    read(wkp_fd, ack_mes, HANDSHAKE_BUFFER_SIZE);
  //  printf("read message: %s\n", ack_mes);

    printf("Response connection is established with client \n" );
    remove("WKP");

    *to_client = pvt_fd;
    return wkp_fd;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int pvt_fd;
  if ((pvt_fd = mkfifo("PRVP", 0644)) == -1) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }

  printf("Trying to connect to well known pipe\n");
    int wkp_fd;
    wkp_fd = open("WPK", O_WRONLY);

  printf("Connected to WKP\n");

  printf("Private pipe opened and writing it's file to the well known pipe\n");

  pvt_fd = open("PRVP", O_RDONLY);

    printf("Waiting for response from server\n");
    char ack_mes[HANDSHAKE_BUFFER_SIZE];
    read(pvt_fd, ack_mes, HANDSHAKE_BUFFER_SIZE);

    printf("Response recieved from server, sending message\n" );

    write(wkp_fd, ack_mes, HANDSHAKE_BUFFER_SIZE);

    printf("Message sent. Connection established with server\n");

    remove("PRVP");
    *to_server = wkp_fd;
    return pvt_fd;
}

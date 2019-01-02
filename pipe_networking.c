#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("making WKP pipe\n");
   mkfifo("to_server",0644);

   printf("Connecting to WKP\n");
   printf("Waiting for client to respond\n");

   int wkp_fd = open("to_server",O_RDONLY);
   printf("Connected to WKP\n");

   char pipe_n[HANDSHAKE_BUFFER_SIZE];
   read(wkp_fd, pipe_n, HANDSHAKE_BUFFER_SIZE);
   remove("to_server");

   int pvt_fd = open(pipe_n,O_WRONLY);
   printf("Sending message to client \n" );
   write(pvt_fd,ACK,HANDSHAKE_BUFFER_SIZE);
   printf("sent client acknowledgement message %s\n",ACK);
   printf("waiting for response from client\n");

   char ack_mes[HANDSHAKE_BUFFER_SIZE];
   read(wkp_fd, ack_mes, HANDSHAKE_BUFFER_SIZE);
   printf("Response connection is established with client \n" );
   printf("read message %s\n", ack_mes);

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

    mkfifo("to_client",0644);

    printf("made private pipe\n");

    int wkp_fd = open("to_server",O_WRONLY);
    char pipe_n[HANDSHAKE_BUFFER_SIZE] = "to_client";
    write(wkp_fd,pipe_n,HANDSHAKE_BUFFER_SIZE);
    printf("sent the file name\n");

    int pvt_fd = open("to_client",O_RDONLY);
    char ack_mes[HANDSHAKE_BUFFER_SIZE];
    read(pvt_fd,ack_mes,HANDSHAKE_BUFFER_SIZE);
    printf("Response recieved from server, sending message\n" );
    printf("read message %s\n", ack_mes);
    remove("to_client");

    write(wkp_fd, ack_mes, HANDSHAKE_BUFFER_SIZE);
    printf("Message sent. Connection established with server\n");

    *to_server = wkp_fd;
    return pvt_fd;

}

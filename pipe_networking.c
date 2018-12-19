#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Creating well known pipe\n");
    int pipe_f;
    if ((pipe_f = mkfifo("WKP", 0644)) == -1) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }
  printf("Connecting to WKP\n");
  while(1){
    int fd;
    if(fd = open("WKP", O_RDONLY) == -1){
      printf("Error %d: %s\n", errno, strerror(errno));
    }

    printf("Connected to WKP\n");

    char * msg = malloc(BUFFER_SIZE);
    read(fd, msg, BUFFER_SIZE);
    printf("Client said: %s", msg);
    close(fd);

    printf("Sending message\n" );
    fd = open("WKP", O_WRONLY);
    char S_client[BUFFER_SIZE];
    fgets(S_client, BUFFER_SIZE, stdin);
    write(fd, S_client, BUFFER_SIZE);

    close(fd);
  }

  return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int pipe_f;
  if ((pipe_f = mkfifo("PRVP", 0644)) == -1) {
  //  printf("Error %d: %s\n", errno, strerror(errno));
  }
  int fd;
  while (1) {
    fd = open("PRVP", O_WRONLY);

    char s_msg[BUFFER_SIZE];
    fgets(s_msg, BUFFER_SIZE, stdin);
    write(fd, s_msg, BUFFER_SIZE);

    close(fd);

    fd = open("PRVP", O_RDONLY);

    char * buf = malloc(BUFFER_SIZE);
    read(fd, buf, BUFFER_SIZE);
    printf("Server said: %s", buf);

    close(fd);
  }
  return 0;
}

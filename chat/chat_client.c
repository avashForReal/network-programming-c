#include <arpa/inet.h> //gives fcn to convert text address ipv4 or v6  into binary
#include <netinet/in.h> //consts and structs for internet domain addresses: eg sockaddr_in - IPv4
#include <stdio.h>  //fcns for i/o
#include <stdlib.h> //defines macros, functions and stuff for general functions
#include <string.h> //for manipulating array of characters
#include <strings.h>
#include <sys/socket.h> //strucures required for sockets
#include <sys/types.h> //data types for system calls, required for running sockets and internet domain addresses
#include <unistd.h>    //POSIX stuff

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  // check for supplied arguments for command line
  if (argc < 3) {
    printf("Usage: <filename> <host_address> <port>");
    exit(1);
  }

  int sock_fd, port, rw_res;
  char buffer[255];
  struct sockaddr_in serv_addr;
  port = atoi(argv[2]);
  sock_fd = socket(AF_INET, SOCK_STREAM, 0); // 0 defaults for tcp

  if (sock_fd < 0) {
    error("Error creating socket!");
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // if(inet_pton(int __af, const char *restrict __cp, void *restrict __buf))
  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    error("Error on processing provided host IP!");
  }

  // if(connect(int __fd, const struct sockaddr *__addr, socklen_t __len))
  if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("Failed while connecting");
  }

  while (1) {
    bzero(buffer, 255);
    // read from the std input and store in the buffer
    fgets(buffer, 255, stdin);

    // write
    rw_res = write(sock_fd, buffer, strlen(buffer));

    if (rw_res < 0) {
      error("Error on reading!");
    }

    bzero(buffer, 255);

    //   must have corresponding write function in client. obvious
    rw_res = read(sock_fd, buffer, 255);

    printf("Server: %s \n", buffer);

    if (rw_res < 0) {
      error("Error on writing!");
    }

    int i = strncmp("bye", buffer, 3);
    if (i == 0) {
      break;
    }
  }
  close(sock_fd);
  return 0;
}
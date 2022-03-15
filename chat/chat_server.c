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
  if (argc < 2) {
    printf("Port is not provided. Terminated!");
    exit(1);
  }

  int sock_fd, new_fd, port;
  // to store returned value from read() and write()
  int rw_res;
  int bind_res;
  char buffer[255]; // to store messages

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t cli_len; // at least 32 bits ko game ho yo

  // sock_fd = socket(int __domain, int __type, int __protocol)
  sock_fd = socket(AF_INET, SOCK_STREAM, 0); // 0 defaults for tcp

  if (sock_fd < 0) {
    error("Error creating socket!");
  }

  // fill serv_addr with zero before filling actual data
  // bzero erases the data in the n bytes of the memory starting at the location
  // pointed
  bzero((char *)&serv_addr, sizeof(serv_addr));
  port = atoi(argv[1]); // converts string into integer

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr =
      htonl(INADDR_ANY); // structure inside structure
                         // host to n/w short, s->short->for port, l->long->for
                         // ip addresses
  serv_addr.sin_port = htons(port);

  // bind_res = bind(int __fd, const struct sockaddr *__addr, socklen_t __len)
  bind_res = bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (bind_res < 0) {
    error("Binding failed!");
  }

  listen(sock_fd, 5); // at most 5 clients in queue

  cli_len = sizeof(cli_addr);

  // new_fd = accept(int __fd, struct sockaddr *restrict __addr, socklen_t
  // *restrict __addr_len)
  new_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);

  if (new_fd < 0) {
    error("Error on accept!");
  }

  while (1) {
    bzero(buffer, 255);
    //   must have corresponding write function in client. obvious
    rw_res = read(new_fd, buffer, 255);
    if (rw_res < 0) {
      error("Error on reading!");
    }
    printf("Client: %s \n", buffer);
    bzero(buffer, 255);
    // read from the std input and store in the buffer
    fgets(buffer, 255, stdin);

    // write
    rw_res = write(new_fd, buffer, strlen(buffer));

    if (rw_res < 0) {
      error("Error on writing!");
    }

    int i = strncmp("bye",buffer,3);
    if(i == 0){
        break;
    }
  }

  close(new_fd);
  close(sock_fd);

  return 0;
}

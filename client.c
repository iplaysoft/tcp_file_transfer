//This is the client of file transfer 
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 64

#ifndef DEBUG_CLIENT
#define DEBUG_CLIENT 1
#endif

typedef struct sockaddr SA;

int main(int argc, const char *argv[])
{
  int nbyte;
  int sockfd, fd;
  char buf[N];
  struct sockaddr_in seraddr;

  if (argc < 3)
    {
      printf ("Usage: %s <ip> <port>\n", argv[0], argv[1]);
      exit (-1);
    }

  bzero (&seraddr, sizeof(seraddr));
  seraddr.sin_family = PF_INET;
  seraddr.sin_port = htons(atoi(argv[2]));
  seraddr.sin_addr.s_addr = inet_addr(argv[1]);

  if (( sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("fail to socket\n");
      exit (-1);
    }

  if (connect(sockfd, (SA *)&seraddr, sizeof(seraddr)) < 0)
    {
      perror("fail to connect\n");
      exit (-1);
    }

  printf("please input filename:");
  fgets(buf, N, stdin);
  buf[strlen(buf)-1] = '\0';
  send(sockfd, buf, N , 0);

  if ((fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0)
    {
      perror("fail to open \n");
      exit (-1);
    }

  while ((nbyte = recv(sockfd, buf, N , 0)) > 0)
    write(fd, buf, nbyte);

  close(fd);
  close(sockfd);
  
  return 0;
}

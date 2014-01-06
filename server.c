//This is the server of file transfer by tcp protocol 
//write by bond, started at 2014.01.03

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef DEBUG_SERVER
#define DEBUG_SERVER 1
#endif

#define N 64
typedef struct sockaddr SA;


int main(int argc, const char *argv[])
{
        int sockfd, cfd, fd;
        char buf[64];
        int nbyte;
	struct sockaddr_in my_addr, peer_addr;
	socklen_t peer_addr_size;

   //    int socket(int domain, int type, int protocol);
     if (( sockfd =socket(AF_INET,SOCK_STREAM , 0)) == -1 )
	 {
	 	printf ("socket is failed\n");
		exit(0);
	 }	 
	 
	 memset(&my_addr, 0, sizeof(my_addr));//clear structure
	 my_addr.sin_family = AF_INET;
	 my_addr.sin_port = htons(atoi(argv[2]));
	 my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	 
//int bind(int sockfd, const struct sockaddr *addr   socklen_t addrlen);

	if (bind( sockfd, ( SA *) &my_addr, sizeof(my_addr)) < 0)
	{
		printf ("bind is failed;\n");
		exit(0);
	
	}
	if (listen(sockfd, 10) < 0)
	{
		printf ("failed at listen\n");
		exit(0);
	}

        peer_addr_size = sizeof (peer_addr);

        while (1)
          {
            if ((cfd = accept(sockfd, (SA *)&peer_addr, &peer_addr_size)) < 0)
              {
                perror("fail to connect\n");
                exit(-1);
              }
              recv(cfd, buf, N, 0);
            fd = open(buf, O_RDONLY);

            while ((nbyte = read(fd, buf, N)) > 0)
              send(cfd, buf, nbyte, 0);
                   close(fd);
                   close(cfd);
          }
	return 0;
}

#include <unistd.h>
#include <sys/types.h>     
#include <sys/socket.h>     
#include <sys/time.h>     
#include <time.h>     
#include <fcntl.h>     
#include <netinet/in.h>     
#include <arpa/inet.h>     
#include <sys/errno.h>     
#include <stdio.h>
#include <stdlib.h>     
#include <stdio.h>     
#include <string.h>

const int MAXLINE = 1024;

int main(int argc,char* argv[])    
{
	int sockfd,n;    
	char recvline[MAXLINE+1];    
	struct sockaddr_in servaddr;    
	if(argc < 2)    
	{    
		printf("usage: a.out<IPaddress");
		exit(0);    
	}

	const char* ip = argv[1];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);    
	if(sockfd < 0)    
	{    
		printf("socket error");
		exit(0);    
	}    
	memset(&servaddr, 0, sizeof(servaddr));    
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(54321);     

	if(inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) 
	{    
		printf("inet_ptons error");
		exit(0);    
	}
	if(connect(sockfd,( struct sockaddr* )&servaddr,sizeof(servaddr)) < 0)    
	{    
		printf("connect error");
		exit(0);    
	}

	while((n=read(sockfd,recvline,MAXLINE)) > 0)    
	{    
		recvline[n]=0;    
		if(fputs(recvline,stdout)==EOF)    
		{    
			printf("fputs error");
			exit(0);    
		}
	}

	close(sockfd);

	if(n < 0)
	{    
		printf("read error");
		exit(0);    
	}    
	exit(0);    
}

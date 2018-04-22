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
#include <errno.h>
#include <string.h>

const int MAXLINE=1024;

int main(int argc,char* argv[])    
{    
    int listenfd,connfd;    
    struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
 
    char buff[MAXLINE];    
    time_t ticks;

	printf("before createing socket...\n");    
    listenfd=socket(AF_INET,SOCK_STREAM,0); 
	printf("after creating socket...\n");    
    
	if(listenfd < 0)    
    {    
        printf("socket error ");
        exit(0);    
    }    
    memset(&servaddr,0, sizeof(servaddr));    
    servaddr.sin_family=AF_INET;    
    servaddr.sin_port=htons(54321);    
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    
	int bindfd = bind(listenfd,( struct sockaddr* )&servaddr,sizeof(servaddr));  
    if(bindfd < 0)    
    {    
        printf("bind error, port is using....");
        exit(0);    
    }
	
    listen(listenfd,MAXLINE);
	printf("waited connection...\n");    

    //time_t rawtime;
    //struct tm* timeinfo;
    //time(&rawtime);
    //char* daytime = asctime(localtime(&rawtime));

	socklen_t client_addrlength = sizeof(clientaddr); 
	connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &client_addrlength);
	char remote[512];
	inet_ntop(AF_INET, &clientaddr.sin_addr, remote, 512);
	int port = ntohs(clientaddr.sin_port);	
	printf("connected with ip: %s and port %d\n", remote, port);

    while(1)      
    {  
        ticks=time(NULL);    
        snprintf(buff, sizeof(buff),"%.24s\n",ctime(&ticks));    
		printf("bufsize: %d daytime=%s\n", (int)strlen(buff), buff);
        write(connfd,buff,strlen(buff));
        //close(connfd);
		sleep(1);
    }
	close(connfd);
    return 0;    
}

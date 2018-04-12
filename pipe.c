#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    char** w_buf[256];
    int childexit=0;
    int i;
    int cmd;
     
    memset(r_buf,0,sizeof(r_buf));
    if(pipe(pipe_fd)<0)
    {
        printf("pipe create error\n");
        return -1;
    }
    if((pid=fork())==0) //child: resolve command from pipe and process
    {
        printf("child process\n");
        close(pipe_fd[1]);
        sleep(2);
        
/* 
        while(!childexit)
        {   
            read(pipe_fd[0],r_buf,4);
            cmd=atoi(r_buf);
            if(cmd==0)
            {
                printf("child: receive command from parent over\n now child process exit\n");
                childexit=1;
            }
            else if(handle_cmd(cmd)!=0)
                return;
            sleep(1);
        }
*/		
		char readbuffer[30];
		//for(i = 0; i < 15; ++i)
		//{
		while(1)
		{
			sleep(1);
			read(pipe_fd[0], readbuffer, 30);
			printf("%s\r\n", readbuffer);
		}
		//}
        close(pipe_fd[0]);
        exit(0);
    }
    else if(pid>0) //parent: send commands to child
    {
        printf("parent process\n");
        close(pipe_fd[0]);
/*  
        w_buf[0]="003";
        w_buf[1]="005";
        w_buf[2]="007";
        w_buf[3]="008";
        for(i=0;i<4;i++)
            write(pipe_fd[1],w_buf[i],4);
*/
		time_t rawtime;
		struct tm* timeinfo;


		//for(i = 0; i < 15; i++)
		//{
		while(1)
		{
        	sleep(1);
			
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			char* timestr = asctime(timeinfo);
			write(pipe_fd[1], timestr, 30);		
		}
		//}
        close(pipe_fd[1]);
    }
	
	getchar();
    return 0;
}


int handle_cmd(int cmd)
{
    if((cmd<0)||(cmd>256)) //suppose child only support 256 commands
    {
        printf("child: invalid command \n");
        return -1;
    }
    printf("child: the cmd from parent is %d\n", cmd);
    return 0;
}
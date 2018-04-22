#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 5
#define BUFF_SIZE 128

int main(int argc, const char* argv[])
{
	struct epoll_event event, events[MAX_EVENTS];
	int running = 1, event_ready_count;
	char read_buffer[BUFF_SIZE + 1];
	size_t bytes_read;

	int epollfd = epoll_create(1);

	printf("epoll fd: %d\n", epollfd);
	if(epollfd == -1)
	{
		fprintf(stderr, "failed to create epoll file descriptor\n");
		return 1;
	}

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fileno(stdin);

	// watch the interesting event in stdin
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fileno(stdin), &event))
	{
		fprintf(stderr, "failed to add file descriptor to epoll\n");
		close(epollfd);
		return 1;
	}

	while(running)
	{
		printf("\nPolling for input...\n");
		event_ready_count = epoll_wait(epollfd, events, MAX_EVENTS, 30000);

		printf("ready event: %d\n", event_ready_count);

		for(int i = 0;i < event_ready_count; ++i)
		{
			printf("Reading from fd [%d]--", events[i].data.fd);
			// read from fd
			bytes_read = read(events[i].data.fd, read_buffer, BUFF_SIZE);
			printf("[%zd] bytes,", bytes_read);

			read_buffer[bytes_read] = '\0';
			printf("data %s", read_buffer);

			if( !strncmp(read_buffer, "stop\n", 5) )
				running = 0;
		}
	}



	if(close(epollfd))
	{
		fprintf(stderr, "failed to close epoll file descriptor\n");
		return 1;
	}

	return 0;
}

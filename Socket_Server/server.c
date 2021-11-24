/************************************************************************
*
*	SERVER PROGRAM
*	Brief: Program to send sensor data received using message queues to CLIENT.
*
*   Author: Dhruv Mehta
*	Code References: https://beej.us/guide/bgnet/html/
*	Beej's Guide to Network Programming
*
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <time.h>
#include <signal.h>

#include <mqueue.h>

#define PORT 		"9000"
#define BACKLOG		5
#define BUF_SIZE	100

/* Socket File descriptor */
int socket_fd;
bool quitpgm = 0;
struct mq_attr sendmq;

struct fnparam
{
	int f_client_fd;
	char f_IP[20];
};
    
mqd_t mq_receive_desc;
int mq_receive_len;
//char buffer[ sizeof(double)];
unsigned int rx_prio;


static void sighandler(int signo)
{
	if((signo == SIGINT) || (signo == SIGTERM))
	{
		if(shutdown(socket_fd, SHUT_RDWR))
		{
			perror("Failed on shutdown()");
		}
		quitpgm = 1;
	}

}

void TxRxData(void *thread_param)
{
	struct fnparam *l_fnp = (struct fnparam*) thread_param;

	char txbuf[20];

	while(1)
	{
		mq_receive_len = mq_receive(mq_receive_desc, txbuf, sizeof(double), &rx_prio);
		if(mq_receive_len < 0)
			perror("Did not receive any data");
		
		/* Send data read from file to client */
		int sent_bytes = send(l_fnp->f_client_fd, txbuf, sizeof(double), 0);
		
		/* Error in sending */
		if(sent_bytes == -1)
		{
			perror("send failed\n");
			return;
		}
	}



} // TxRxThread end


int main(int argc, char* argv[])
{
	struct addrinfo *sockaddrinfo;
	struct addrinfo hints;
	struct sockaddr_in clientsockaddr;
	socklen_t addrsize = sizeof(struct sockaddr);
	int opt = 1;
	int client_fd; 
    	struct fnparam f_param;
	char IP[20] = {0};

	memset(&hints, 0, sizeof(hints));

	openlog(NULL, 0, LOG_USER);

	if(argc != 2)
	{
		printf("usage socketserver [IP]\n");	
		return -1;
	}

	memcpy(IP, argv[1], strlen(argv[1]));
	printf("Server's IP address: %s\n",IP);

	/* Setting up signal handlers SIGINT and SIGTERM */
	if(signal(SIGINT, sighandler) == SIG_ERR)
	{
		printf("Cannot handle SIGINT\n");
		return -1;
	}

	if(signal(SIGTERM, sighandler) == SIG_ERR)
	{
		printf("Cannot handle SIGTERM\n");
		return -1;
	}

  	/* Create socket endpoint */ 
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	/* Socket creation failed, return -1 on error */
	if(socket_fd == -1)
	{
		printf("Socket creation failed\n");
		return -1;
	}

	 /* Forcefully attaching socket to the port 9000 for bind error: address in use */
    	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
        	perror("setsockopt");
		goto cleanexit;    
	}

	/* Setting this for use with getaddrinfo for bind() */
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags =  AI_NUMERICHOST;

	int rc = getaddrinfo(IP, PORT, &hints, &sockaddrinfo);
	/* Error occurred in getaddrinfo, return -1 on error */
	if(rc != 0)
	{
		printf("getaddrinfo failed, %s\n", gai_strerror(rc));
		freeaddrinfo(sockaddrinfo);
		goto cleanexit;
	}
	/* Bind */
	rc = bind(socket_fd, sockaddrinfo->ai_addr, sizeof(struct sockaddr));

	/* Error occurred in bind, return -1 on error */
	if(rc == -1)
	{
		printf("bind failed, %s\n", strerror(errno));
		freeaddrinfo(sockaddrinfo);
		goto cleanexit;
	}

	freeaddrinfo(sockaddrinfo);

	/* Listen for a connection */
	rc = listen(socket_fd, BACKLOG);

	/* Error occurred in listen, return -1 on error */
	if(rc == -1)
	{
		perror("listen failed\n");
		goto cleanexit;
	}


	mq_receive_desc = mq_open("/temp_sense_mq", O_RDWR, S_IRWXU, &sendmq);
	if(mq_receive_desc < 0)
	{
		perror("Reciever MQ failed");
		exit(-1);
	} 

	while(!quitpgm)
	{		

		/* Get the accepted client fd */
		client_fd = accept(socket_fd, (struct sockaddr *)&clientsockaddr, &addrsize); 

		if(quitpgm)
			break;
		
		/* Error occurred in accept, return -1 on error */
		if(client_fd == -1)
		{
			perror("accept failed\n");
			goto cleanexit;
		}
		
		/* sockaddr to IP string */
		char *IP = inet_ntoa(clientsockaddr.sin_addr);
		/* Log connection IP */
		syslog(LOG_DEBUG, "Accepted connection from %s\n", IP);

		f_param.f_client_fd = client_fd;
		strcpy(f_param.f_IP, IP);
		
		TxRxData(&f_param);
	}

cleanexit:

    	syslog(LOG_DEBUG, "Caught signal, exiting\n");
	close(client_fd);
	close(socket_fd);
    	syslog(LOG_DEBUG, "Closed connection from %s\n", f_param.f_IP);

	return 0;

}

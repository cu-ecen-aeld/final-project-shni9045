/*
*
*	Filename: server.c 
*	
*       Description : Program to send sensor data received from message queue to the client.
*
*  
*	Code References: https://beej.us/guide/bgnet/html/
*	
*
*/
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
#include <mqueue.h>
#include <signal.h>


#define PORT 		"9000"
#define BACKLOG		5
#define BUF_SIZE	100


int sock_fd;
int client_fd; 
bool quit = 0;
struct mq_attr transmq;

struct fparam
{
	int client_id;
	char f_InternetProtocol[20];
};
    

mqd_t mq_rx_id;
int mq_rx_len;
unsigned int receive_priority;

void close_graceful(){

   
    	syslog(LOG_DEBUG, "Caught signal, exiting\n");
	close(client_fd);
	close(sock_fd);
    	syslog(LOG_DEBUG, "Closed connection from ");

}


static void sighandler(int signo)
{
	if((signo == SIGINT) || (signo == SIGTERM))
	{
		if(shutdown(sock_fd, SHUT_RDWR))
		{
			perror("Failed on shutdown()");
		}
		quit = 1;
	}

}




int main(int argc, char* argv[])
{
	struct addrinfo *sockaddrinfo;
	struct addrinfo hints;
	struct sockaddr_in clientsockaddr;
	socklen_t addrlength = sizeof(struct sockaddr);
	int opt = 1;
    	struct fparam f_param;
	char IP[20] = {0};
     

	char txbuf[sizeof(double)+sizeof(int)];
	
	char buffer[100];
	
	double temperature;
	int id;

	memset(&hints, 0, sizeof(hints));

	openlog(NULL, 0, LOG_USER);

	if(argc != 2)
	{
		printf("socketserver IP usage\n");	
		return -1;
	}

	memcpy(IP, argv[1], strlen(argv[1]));
	printf("IP address of Server is: %s\n",IP);

	/* Set up signal handlers for signals  SIGINT and SIGTERM */
	if(signal(SIGINT, sighandler) == SIG_ERR)
	{
		printf("SIGINT failed\n");
		return -1;
	}

	if(signal(SIGTERM, sighandler) == SIG_ERR)
	{
		printf("SIGTERM failed\n");
		return -1;
	}

  	/* Create socket endpoint */ 
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	/* Socket creation failed */
	if(sock_fd == -1)
	{
		printf("Failed to create Socket\n");
		return -1;
	}

	
    	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
        	perror("setsockopt");
		close_graceful();   
	}

	
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags =  AI_NUMERICHOST;

	int rc = getaddrinfo(IP, PORT, &hints, &sockaddrinfo);
	
         /* Error in getaddrinfo */
	if(rc != 0)
	{
		printf("getaddrinfo failed, %s\n", gai_strerror(rc));
		freeaddrinfo(sockaddrinfo);
		close_graceful();
	}


	rc = bind(sock_fd, sockaddrinfo->ai_addr, sizeof(struct sockaddr));

	
	if(rc == -1)
	{
		printf("bind failed, %s\n", strerror(errno));
		freeaddrinfo(sockaddrinfo);
		close_graceful();
	}

	freeaddrinfo(sockaddrinfo);

	
	rc = listen(sock_fd, BACKLOG);

	
	if(rc == -1)
	{
		perror("listen failed\n");
		close_graceful();
	}


	mq_rx_id = mq_open("/temp_sense_mq", O_RDWR, S_IRWXU, &transmq);
        
        /* Message queue open failed */

	if(mq_rx_id < 0)
	{
		perror("Reciever MQ failed");
		exit(-1);
	} 

	while(!quit)
	{		

		/* Obtain client fd */
		client_fd = accept(sock_fd, (struct sockaddr *)&clientsockaddr, &addrlength); 

		if(quit)
			break;
		
		/* Error at accept */
		if(client_fd == -1)
		{
			perror("accept call failed\n");
			close_graceful();
		}
		
		
		char *IP = inet_ntoa(clientsockaddr.sin_addr);
		
		syslog(LOG_DEBUG, "Accepted connection from client %s\n", IP);

		f_param.client_id = client_fd;
		strcpy(f_param.f_InternetProtocol, IP);

               struct fparam *l_fnp = (struct fparam*) &f_param;
               
               while (1){

	        mq_rx_len = mq_receive(mq_rx_id, txbuf, sizeof(double)+sizeof(int), &receive_priority);
		if(mq_rx_len < 0)
			perror("Data not received ");
			
	        memcpy(&temperature, txbuf, sizeof(double));
		memcpy(&id, txbuf + sizeof(double), sizeof(int));
		
		
		sprintf(buffer, "TC%.2lf\nID%02d\n", temperature, id);
		
		/* Send data read from file to client */
		int sent_bytes = send(l_fnp->client_id, buffer, strlen(buffer)+1, 0);
		
		/* Error in sending */
		if(sent_bytes == -1)
		{
			perror("Failed to send Bytes\n");
			break;
		}
	   }
	}

	return 0;

}

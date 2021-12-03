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

struct fnparam
{
	int f_client_fd;
	char f_IP[20];
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
	socklen_t addrsize = sizeof(struct sockaddr);
	int opt = 1;
    	struct fnparam f_param;
	char IP[20] = {0};
     

	char txbuf[sizeof(double)+sizeof(int)];
	
	char buffer[100];
	
	double temperature;
	int id;

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
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	/* Socket creation failed, return -1 on error */
	if(sock_fd == -1)
	{
		printf("Socket creation failed\n");
		return -1;
	}

	 /* Forcefully attaching socket to the port 9000 for bind error: address in use */
    	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
        	perror("setsockopt");
		close_graceful();   
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
		close_graceful();
	}
	/* Bind */
	rc = bind(sock_fd, sockaddrinfo->ai_addr, sizeof(struct sockaddr));

	/* Error occurred in bind, return -1 on error */
	if(rc == -1)
	{
		printf("bind failed, %s\n", strerror(errno));
		freeaddrinfo(sockaddrinfo);
		close_graceful();
	}

	freeaddrinfo(sockaddrinfo);

	/* Listen for a connection */
	rc = listen(sock_fd, BACKLOG);

	/* Error occurred in listen, return -1 on error */
	if(rc == -1)
	{
		perror("listen failed\n");
		close_graceful();
	}


	mq_rx_id = mq_open("/temp_sense_mq", O_RDWR, S_IRWXU, &transmq);
	if(mq_rx_id < 0)
	{
		perror("Reciever MQ failed");
		exit(-1);
	} 

	while(!quit)
	{		

		/* Get the accepted client fd */
		client_fd = accept(sock_fd, (struct sockaddr *)&clientsockaddr, &addrsize); 

		if(quit)
			break;
		
		/* Error occurred in accept, return -1 on error */
		if(client_fd == -1)
		{
			perror("accept failed\n");
			close_graceful();
		}
		
		/* sockaddr to IP string */
		char *IP = inet_ntoa(clientsockaddr.sin_addr);
		/* Log connection IP */
		syslog(LOG_DEBUG, "Accepted connection from %s\n", IP);

		f_param.f_client_fd = client_fd;
		strcpy(f_param.f_IP, IP);

               struct fnparam *l_fnp = (struct fnparam*) &f_param;
               
               while (1){

	        mq_rx_len = mq_receive(mq_rx_id, txbuf, sizeof(double)+sizeof(int), &receive_priority);
		if(mq_rx_len < 0)
			perror("Did not receive any data");
			
	        memcpy(&temperature, txbuf, sizeof(double));
		memcpy(&id, txbuf + sizeof(double), sizeof(int));
		
		
		sprintf(buffer, "TC%.2lf\nID%02d\n", temperature, id);
		
		/* Send data read from file to client */
		int sent_bytes = send(l_fnp->f_client_fd, buffer, strlen(buffer)+1, 0);
		
		/* Error in sending */
		if(sent_bytes == -1)
		{
			perror("send failed\n");
			break;
		}
	   }
	}

	return 0;

}

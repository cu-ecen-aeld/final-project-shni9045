/**************************************************************************************
* Server TCP Program
* @file - server.c
* Reference: https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
*
***************************************************************************************/
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>

#define MAX 80 
#define PORT 9000 
#define SA struct sockaddr 

#define MQUEUE_NAME  "/temp_sense_mq"

struct mq_attr mq_attr;


void communicate_sensordata(mqd_t mq,int sockfd){
         
         int nbytes;
         int tbytes;
         unsigned int prio;

         char send_buffer[sizeof(double)];
         
         if ((nbytes = mq_receive( mq, send_buffer, sizeof(double), &prio)) == -1 )
          {
             perror("MQ RECEIVE");
             printf("Error in Receiving data over Message Queue\n");  
       
          }
       
          else {
          
          
             printf("Message Received over Message Queue successfully\n");
             
             tbytes = send( sockfd, send_buffer, sizeof(double),0);
             
             if (tbytes == -1 ){
                            
                 printf("Error in Sending Data over Socket\n"); 
             
             }
                        
          }
        
}


// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli;
	mq_attr.mq_maxmsg = 100;
	mq_attr.mq_msgsize = sizeof(double);
    
        mqd_t mymq;
    
        mymq = mq_open(MQUEUE_NAME, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    
        if (mymq < 0 ) {  
                    
              perror("MQ OPEN");
              printf("Error in opening Message Queue\n");
    
       }
    
        else printf("Message Queue opened successfully\n");

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, (socklen_t*)&len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	while (1){
	   
	   communicate_sensordata(mymq,sockfd);
	
	}

	// After chatting close the socket 
	close(sockfd); 
} 


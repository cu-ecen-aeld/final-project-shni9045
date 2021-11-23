/*
* Client code for implementing simple string transmission to server
* Reference: https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
*
*/



#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <unistd.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <netinet/in.h> 

#include <arpa/inet.h>

#define MAX 80
#define PORT 9000
#define SA struct sockaddr


void  receive_sensordata(int sockfd)
{

        int fd,wbytes;
	
	char buff[MAX];

	for (;;) {
	
		bzero(buff, sizeof(buff));

		read(sockfd, buff, sizeof(double));
		
		// Open file to write the converted temperature
    	       fd =  open("/var/tmp/tempdata.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
               if(fd<0){

                 printf("Error in opening file\n");
                 
               }
       
             // Write buffer to file
             wbytes = write(fd,buff,sizeof(double));
             if (wbytes == -1){
                
                printf("Error in writing to file\n");
                 
              }
       
            // Close file descriptor
            close(fd);
		
	}
}

int main()
{


        
	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	char ip_addr[24]={0};
	printf("Enter IP address of the server: ");
	scanf("%s",ip_addr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip_addr);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	receive_sensordata(sockfd);

	// close the socket
	close(sockfd);
}

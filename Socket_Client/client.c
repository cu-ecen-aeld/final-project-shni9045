/* References:
 * 1. library has to be installed: https://stackoverflow.com/questions/61186574/libgpiod-api-usage-linux-shared-libraries
 * 2. client code reference: https://www.geeksforgeeks.org/socket-programming-cc/
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

//client socket related extra additions
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PORT_NO				(9000)
#define MSEC_TO_SEC			(1000)
#define USEC_TO_MSEC			(1000)

#define SLEEP_TIME			(5*(USEC_TO_MSEC)*(MSEC_TO_SEC))




//client related initialization
bool client_init(const char IP[]);

//client global variables
int client_socket_fd;

int main(int argc, char *argv[])
{      
        int fd,wbytes;

	bool client_init_status = false;
	char buffer[1024] = {0};
	char IP[20] = {0};

	if(argc != 2)
	{
		printf("Usage client_test [IP]\n");
		return -1;
	}

	memcpy(IP, argv[1], strlen(argv[1]));
	printf("Connecting to Server's IP %s\n", IP);


	client_init_status = client_init(IP);
	if(client_init_status == false)
	{
		perror("client_init");  
		return -1;
	}

	
	while(1)
	{
		read(client_socket_fd,buffer,sizeof(double));

		// Open file to write the converted temperature
    	       fd =  open("/var/tmp/tempdata.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
               if(fd<0){

                 printf("Error in opening file\n");
                 
               }
       
             // Write buffer to file
             wbytes = write(fd,buffer,sizeof(double));
             if (wbytes == -1){
                
                printf("Error in writing to file\n");
                 
              }
       
            // Close file descriptor
            close(fd);
	}
}

bool client_init(const char IP[])
{
	client_socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(client_socket_fd < 0)
	{
		perror("client_socket_fd");  
		return false; 
	}
	
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NO);
	
	int client_inet_pton_fd = inet_pton(AF_INET,IP,&server_address.sin_addr);
	if(client_inet_pton_fd <= 0)
	{
		perror("client_inet_pton_fd");  
		return false; 
	}
	
	int client_connect_fd = connect(client_socket_fd, (struct sockaddr *)&server_address,sizeof(server_address));
	if(client_connect_fd < 0)
	{
		perror("client_connect_fd");  
		return false; 
	}

	printf("client init completed\n");

	return true;
}




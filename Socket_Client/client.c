/* 
 * Attributes :
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

/* GPIO Macro Definitions*/
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN  20            // GPIO N0 20

#define POUT 21            // GPIO NO 21


#define PORT_NO				(9000)
#define MSEC_TO_SEC			(1000)
#define USEC_TO_MSEC			(1000)

#define SLEEP_TIME			(5*(USEC_TO_MSEC)*(MSEC_TO_SEC))


// Cient Socket fd
int clientfd;

// Variable to start or stop accepting connections
int shutoff=0;	

void close_graceful(){

	shutoff = 1;

	close(clientfd);

	printf("-------------------Exiting Program--------------\n");

}

static void sig_handler(int signo){


    if(signo == SIGINT || signo==SIGTERM) {


    shutdown(clientfd,SHUT_RDWR);

    shutoff = 1;

    
    }

}

static int GPIOExport(int pin)
{
        #define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

        #define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}



int main(int argc, char *argv[])
{   

	int id;
	float temperature;     
	
	char buffer[1024] = {0};
	char hold_str[200] = {0};
	char temp_str[20];	
	char conv_str[20];
	char IP[20] = {0};	

	int fd,td,wbytes;
	
	/*
	 * Enable GPIO pins
	 */
	if (-1 == GPIOExport(POUT) || -1 == GPIOExport(PIN))
		return(1);

	/*
	 * Set GPIO directions
	 */
	if (-1 == GPIODirection(POUT, OUT) || -1 == GPIODirection(PIN, OUT))
		return(2);
		
		

	if(argc != 2)
	{
		printf("Enter Server IP Please:\n");
		return -1;
	}

	
	memcpy(IP, argv[1], strlen(argv[1]));
	printf("________________CONNECTING TO SERVER____________________ %s\n", IP);


	clientfd = socket(AF_INET,SOCK_STREAM,0);
	if(clientfd < 0)
	{
		perror("ERROR Socket Creation:");  
	}
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT_NO);
	
	int inetfd = inet_pton(AF_INET,IP,&serveraddr.sin_addr);
	if(inetfd <= 0)
	{
		perror("ERROR inet pton() call:");  
		close_graceful();
		return -1;
		
	}
	
	int connection = connect(clientfd, (struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(connection < 0)
	{
		perror("ERROR Connect() call:");  
		close_graceful();
		return -1;

	}

	printf("_______________INITIALIZATION OF CLIENT COMPLETED___________\n");

	// Setup SIGINT signal handler
    if(signal(SIGINT,sig_handler) == SIG_ERR){
        printf("\nError in setting signals");
        close_graceful();
        return -1;
    }
    
    // Setup SIGTERM signal handler
    if(signal(SIGTERM,sig_handler) == SIG_ERR){
        printf("\nError in setting signals");
        close_graceful();
        return -1;
    }

    // Setup signal mask
    sigset_t set;
    sigemptyset(&set);           // empty the set

    // add the signals to the set
    sigaddset(&set,SIGINT);      
    sigaddset(&set,SIGTERM);     


	
	while(!shutoff)
	{
		
		read(clientfd,buffer,sizeof(buffer));
			
		printf("-----------------FROM SERVER-----------------%s\n",buffer);
		
		strncpy(hold_str,buffer,strlen(buffer));
			
		if(!(strncmp(hold_str,"TC",2))){
			
			char buf[10] = {0};
		
		
			strncpy(temp_str,hold_str,7);			
			strncpy(conv_str,temp_str+2,5);		
			temperature = atof(conv_str);
		  
		    strncpy(temp_str,"",strlen(temp_str));
		  
	    	printf("----Converted Temperature : %.2lf",temperature);
		
			sprintf(buf,"%04.2f",temperature);
		  
		
		
		    // Open file to write the converted temperature
			fd =  open("/var/tmp/tempdata.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
			if(fd<0){

			printf("Error in opening file\n");
			
			}

			// Write buffer to file
			wbytes = write(fd,buf,strlen(buf));
			if (wbytes == -1){
		
			printf("Error in writing to file\n");
			
			}

			// Close file descriptor
			close(fd);
		
		
		}
		
		
		if (!(strncmp(hold_str+8,"ID",2))){
			
			char buf[10] = {0};
		  
		    strncpy(temp_str,hold_str+8,4);
		  
			strncpy(conv_str,temp_str+2,2);

			
			id = atoi(conv_str);
			
			strncpy(temp_str,"",strlen(temp_str));
			
			
			printf("----Converted ID: %d",id);
			
			sprintf(buf,"%d",id);
			
		    // Open file to write the converted temperature
    	     td =  open("/var/tmp/iddata.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
             if(td<0){

                 printf("Error in opening file\n");
                 
               }
       
             // Write buffer to file
             wbytes = write(td,buf,strlen(buf));
             if (wbytes == -1){
                
                printf("Error in writing to file\n");
                 
              }
       
            // Close file descriptor
            close(td);
			
		}
		                  
	}

	close_graceful();
    return 0;

}




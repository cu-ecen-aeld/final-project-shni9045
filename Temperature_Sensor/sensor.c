/* 
* Source file containing implementation of Client Sensor Communication
* Attributes : https://olegkutkov.me/2017/08/10/mlx90614-raspberry/
*              https://classes.colorado.edu/?keyword=ECEN%205623&srcdb=2214 (message queues)
*
* Modified - shni9045@colorado.edu
* Message Queue Communication : shrikant nimhan
* Temperature Sensor : shrikant nimhan
* Fingerpritn Sensor : chirayu thakur 
* 
*/
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>


#define MQUEUE_NAME  "/temp_sense_mq"

struct mq_attr mq_attr;

#define MLX90614_TA 			(0x06)
#define MLX90614_TOBJ1 			(0x07)
#define MLX90614_TOBJ2 			(0x08)
#define TEMPERATURE_TYPE 		(MLX90614_TOBJ1)

#define MLX90614_DEVICE_ADDRESS		(0x5A)
#define I2C_DEV_PATH 			("/dev/i2c-1")

#define SLEEP_DURATION 			(750000)


#ifndef I2C_SMBUS_READ 
#define I2C_SMBUS_READ 1 
#endif 
#ifndef I2C_SMBUS_WRITE 
#define I2C_SMBUS_WRITE 0 
#endif


#define MAX_FRAME_SIZE          12
#define UARTIO_OK              0
#define ERR_OPEN               -1
#define ERR_CLOSE              -2
#define ERR_READ               -3
#define ERR_WRITE              -4
#define ERR_DATA_MISMATCH      -5
#define MAX 			12

typedef union i2c_smbus_data i2c_data;


unsigned char OPEN[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01};
unsigned char CLOSE[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x01};	
unsigned char LED_ON[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x12, 0x00, 0x13, 0x01};
unsigned char LED_OFF[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x12, 0x01};
unsigned char ISPRESSFINGER2[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x26, 0x00, 0x27, 0x01};

unsigned char ISPRESSFINGER[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x26, 0x01};
unsigned char GETENROLLCOUNT[MAX_FRAME_SIZE]	= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x01};
unsigned char CAPTURE[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x01};
unsigned char CAPTURE_BEST[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x60, 0x00, 0x61, 0x01};
unsigned char IDENTIFY_N[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x51, 0x01};

unsigned char CHECKENROLL[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x21, 0x01};
unsigned char ENROLLSTART[MAX_FRAME_SIZE] 	= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x22, 0x01};
unsigned char ENROLL1[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x23, 0x01};
unsigned char ENROLL2[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x24, 0x01};
unsigned char ENROLL3[MAX_FRAME_SIZE] 		= {0x55, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x25, 0x01};



unsigned char cResponse[MAX_FRAME_SIZE];
char sDevice[] = "/dev/serial0";
unsigned char bytes_read=0;
unsigned char bytes_write=0;
int hDevice=0;
char buff[MAX];

int error(char *sFormat, ...) {
  va_list args;

  // not in quiet mode?
  // output error
  printf("ERROR: ");

  va_start(args, sFormat);
  vprintf(sFormat, args);
  va_end(args);

  printf("\n");
  return -1;
}


void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

int io_open(char *sDevice,int nOpen) {
	// can open it?
	int hDevice=open(sDevice,nOpen);

	if(hDevice<0) {
		error("io failed to open %s",sDevice);
		return -1;
	}
	return hDevice;
}

// io_close
// Call to close device

int io_close(int hDevice) {
	// can close it?
	if(close(hDevice)<0) {
		error("io failed to close device");
		return -1;
	}
	return 0;
}

void usage() {
	printf("\nUsage:\n\n");
	printf("\nExample:\n");
	printf("uart-loopback /dev/ttyO2\n");
	printf("\n");
}


void CRC_value(unsigned char *data)
{
	uint16_t sum =0;
		
	for(int i=0;i<10;i++)
		sum+= data[i];

	data[10] = (sum & 0x00ff);
	data[11] = (sum & 0xff00)>>8;
}


void led_open()
{
	if ((bytes_write=write(hDevice,OPEN,sizeof(OPEN)))!=(sizeof(OPEN))) 
	{
    		io_close(hDevice);
    		error("io failed to write at %s. Bytes Written:%d",sDevice,bytes_write);
  	}
  	// read response
  	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);
  	if (bytes_read<MAX_FRAME_SIZE) 
	{
    		io_close(hDevice);
    		error("io failed to read from %s, bytes read: %d",sDevice, bytes_read);
  	}
}

int led_on_fingerprint()
{
	for (int i=0; i<MAX_FRAME_SIZE; i++) 
    		cResponse[i] = 0;

	bytes_write=write(hDevice,LED_ON,sizeof(LED_ON));
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);
	if(cResponse[8] == 0x30)
		return 0;
	else
		return -1;
}

int led_off_fingerprint()
{
	for (int i=0; i<MAX_FRAME_SIZE; i++) 
    		cResponse[i] = 0;

	bytes_write=write(hDevice,LED_OFF,sizeof(LED_OFF));
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);
	if(cResponse[8] == 0x30)
		return 0;
	else
		return -1;
}

int identify_fingerprint()
{
	for (int i=0; i<MAX_FRAME_SIZE; i++) 
    		cResponse[i] = 0;	

	//Capture fingerprint
	bytes_write=write(hDevice,CAPTURE,sizeof(CAPTURE));
	while(1)
	{
		bytes_read=read(hDevice, cResponse, 1);
		if(cResponse[0] == 0x55)
			break;
	}
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);

	//Identify fingerprint
	for (int i=0; i<MAX_FRAME_SIZE; i++)
    		cResponse[i] = 0;
	bytes_write=write(hDevice,IDENTIFY_N,sizeof(IDENTIFY_N));
	while(1)
	{
		bytes_read=read(hDevice, cResponse, 1);
		if(cResponse[0] == 0x55)
			break;
	}
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);

	if(cResponse[7] == 0x30)
	{
	 
	 if(cResponse[3] == 1)
        {
            printf("Fingerprint ID: Chirayu\n");
            buff[0] = (char) cResponse[3];
	    while(led_off_fingerprint() == -1);
	    return 0;
        } 
        
        if(cResponse[3] == 2)
        {
            printf("Fingerprint ID: Shrikant\n");
            buff[0] = (char) cResponse[3];
	    while(led_off_fingerprint() == -1);
	    return 1;

		
	}
	
	}


	buff[0] = (char)0;
	while(led_off_fingerprint() == -1);
	return 200;
}

int is_Finger_pressed()
{
	for (int i=0; i<MAX_FRAME_SIZE; i++) 
    		cResponse[i] = 0;

        int pval;
	bytes_write=write(hDevice,ISPRESSFINGER,sizeof(ISPRESSFINGER));
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);
	//if(cResponse[8] == 0x30 && cResponse[7] == 0x00 && cResponse[6] == 0x00 && cResponse[5] == 0x00 && cResponse[4] == 0x00)
	//{	
		//return 1;
	//}
        if(cResponse[8] == 0x30)
        {
           pval=cResponse[4];
           pval+=cResponse[5];
           pval+=cResponse[6];
           pval+=cResponse[7];
        }
        if(pval==0) 
           return 1;
	return 0;
}

int get_fingerprint_count()
{
	uint32_t count = 0;
	for (int i=0; i<MAX_FRAME_SIZE; i++) 
    		cResponse[i] = 0;

	bytes_write=write(hDevice,GETENROLLCOUNT,sizeof(GETENROLLCOUNT));
	bytes_read=read(hDevice, cResponse, MAX_FRAME_SIZE);

	if(cResponse[8] == 0x30)
	{
		count += cResponse[4];
		count += (cResponse[5]>>8)&0x0000ff00;
		count += (cResponse[6]>>16)&0x00ff0000;
		count += (cResponse[7]>>24)&0xff000000;
		return count;
	}
	return 0;
}


int main()
{

    int nbytes;
     
    int fdev = open(I2C_DEV_PATH, O_RDWR); // open i2c bus

    if (fdev < 0) {
        fprintf(stderr, "Failed to open I2C interface %s Error: %s\n", I2C_DEV_PATH, strerror(errno));
        return -1;
    }
    
    // set slave device address, default MLX is 0x5A
    unsigned char i2c_addr = MLX90614_DEVICE_ADDRESS;
    if (ioctl(fdev, I2C_SLAVE, i2c_addr) < 0) {
        fprintf(stderr, "Failed to select I2C slave device! Error: %s\n", strerror(errno));
        return -1;
    }

    // enable checksums control
    if (ioctl(fdev, I2C_PEC, 1) < 0) {
        fprintf(stderr, "Failed to enable SMBus packet error checking, error: %s\n", strerror(errno));
        return -1;
    }

    // trying to read something from the device using SMBus READ request
    i2c_data data;
    char command = TEMPERATURE_TYPE; 
    // build request structure
    struct i2c_smbus_ioctl_data sdat = {
        .read_write = I2C_SMBUS_READ,
        .command = command,
        .size = I2C_SMBUS_WORD_DATA,
        .data = &data
    };
    
    char send_buffer[sizeof(double)+sizeof(int)];

    mq_attr.mq_maxmsg = 100;
    mq_attr.mq_msgsize = sizeof(double)+sizeof(int);
    
    mqd_t mymq;
    
    mymq = mq_open(MQUEUE_NAME, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    
    if (mymq < 0 ) {
       
       perror("MQ OPEN");
       printf("Error in opening Message Queue\n");
    
    }
    
    else printf("Message Queue opened successfully\n");

    hDevice=io_open(sDevice,O_RDWR | O_NOCTTY);
	if(hDevice<0) 
	{
    		return ERR_OPEN;
  	}
  
  // Set attributes
  	struct termios options;
	tcgetattr(hDevice, &options);
	options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	options.c_cflag &= ~(CSIZE | PARENB);
	options.c_cflag |= B9600 | CS8 | CREAD | CLOCAL;
	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 0;
	tcflush(hDevice, TCIOFLUSH);
	tcsetattr(hDevice, TCSANOW, &options);
  
	bzero(buff, sizeof(buff));
	led_open();
    

    while(1)
    {

	if (ioctl(fdev, I2C_SMBUS, &sdat) < 0) {
       	fprintf(stderr, "Failed to perform I2C_SMBUS transaction, error: %s\n", strerror(errno));
        return -1;
    	}
	
	// Temperature COnversion as per datasheet
     	double temp = (double) data.word;
    	temp = (temp * 0.02)-0.01;
    	temp = temp - 273.15;
    	
    	// Display result for debug purposes only
    	printf("Body Temperature of Person  = %04.2f\n", temp);
    	
    	led_on_fingerprint();
        printf("Press finger\n");

        /*****Finger Print Sensor******/

        if(is_Finger_pressed())
        {
            printf("Finger is pressed\n");
            int id = identify_fingerprint();

            printf("******ID****** === %d\n",id);

            // Create string buffer of temperature value
            //sprintf(send_buffer,"%04.2f%d",temp,id);

            memcpy(send_buffer, &temp, sizeof(double));
            memcpy(send_buffer + sizeof(double), &id, sizeof(int));

        if ((nbytes = mq_send( mymq, send_buffer, sizeof(double)+sizeof(int), 1)) == -1 )
         {
             perror("MQ SEND");
             printf("Error in  Sending data over  Message Queue\n");  
             return -1;
       
         }
       
          else printf("Message data sent over Message Queue successfully\n");       

         }					

    	usleep(SLEEP_DURATION);



    }

    // close device
    io_close(hDevice);

    return 0;
}

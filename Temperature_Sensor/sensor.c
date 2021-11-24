/* 
* Source file containing implementation of user space I2C driver for MLX90614 body temperature sensor
* Attributes : https://olegkutkov.me/2017/08/10/mlx90614-raspberry/
*
* Modified - shni9045@colorado.edu
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

typedef union i2c_smbus_data i2c_data;

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
    
    char send_buffer[sizeof(double)];
    
    mq_attr.mq_maxmsg = 100;
    mq_attr.mq_msgsize = sizeof(double);
    
    mqd_t mymq;
    
    mymq = mq_open(MQUEUE_NAME, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    
    if (mymq < 0 ) {
       
       perror("MQ OPEN");
       printf("Error in opening Message Queue\n");
    
    }
    
    else printf("Message Queue opened successfully\n");
    

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
    	
        // Create string buffer of temperature value
       sprintf(send_buffer,"%04.2f",temp);
       
       if ((nbytes = mq_send( mymq, send_buffer, sizeof(double), 1)) == -1 )
       {
             perror("MQ SEND");
             printf("Error in  Sending data over  Message Queue\n");  
             return -1;
       
       }
       
       else printf("Message data sent over Message Queue successfully\n");
    	
       
    	// Display result for debug purposes only
    	printf("Body Temperature of Person  = %04.2f\n", temp);

    	usleep(SLEEP_DURATION);
    }

    return 0;
}

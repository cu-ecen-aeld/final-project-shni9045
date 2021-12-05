/*
* C++ source file containing impplementation of  Values Class to handle updated temperature value, update fingerprint ID and other labels in window
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/

#include <QtWidgets>
#include "values.h"

Values::Values()
{
    // QvBoxLayout object for Various Labels implemented inside GUI
    QVBoxLayout *layout = new QVBoxLayout;
    
    // Label for the application purpose
    QLabel *app_name = new QLabel(tr("AESD SECURE ATTENDANCE"));
    
    // Label formatting throught various private members of QFont
    QFont s = app_name->font();
    s.setPointSize(82);
    s.setBold(true);
    s.setItalic(true);
    app_name->setFont(s);
    
    // Set Label alignment 
    app_name->setAlignment(Qt::AlignLeft);

    
    // Label for the temperature type
    QLabel *temperature_l = new QLabel(tr("USER BODY TEMP.(°C)"));

    // Label formatting throught various private members of QFont
    QFont t = temperature_l->font();
    t.setPointSize(40);
    t.setBold(true);
    temperature_l->setFont(t);

    // Set Label alignment 
    temperature_l->setAlignment(Qt::AlignHCenter);
    
    // Label for the temperature in celsius
    temperature_v = new QLabel();

    // Label formatting throught various private members of QFont
    QFont f = temperature_v->font();
    f.setPointSize(40);
    f.setBold(true);
    temperature_v->setFont(f);
    
    // Set Label alignment 
    temperature_v->setAlignment(Qt::AlignHCenter);

    // Label for the fingerprint status
    QLabel *fingerprint = new QLabel(tr("FINGERPRINT STATUS"));
    
    // Label formatting throught various private members of QFont
    QFont u = fingerprint->font();
    u.setPointSize(40);
    u.setBold(true);
    fingerprint->setFont(u);
    
     // Set Label alignment 
    fingerprint->setAlignment(Qt::AlignHCenter);

    // Label for displaying user
    user = new QLabel();
    
    // Label formatting throught various private members of QFont
    QFont v = user->font();
    v.setPointSize(40);
    v.setBold(true);
    user->setFont(v);
    
    // Set Label alignment 
    user->setAlignment(Qt::AlignHCenter);
    
    
    layout->addWidget(app_name);
    layout->addWidget(temperature_l);
    layout->addWidget(temperature_v);
    layout->addWidget(fingerprint);
    layout->addWidget(user);


    setLayout(layout);
}

/* Member function to update displayed temperature value
*  PARAMETERS : Curent temperature value
*  RETURNS    : NONE
*/

void Values::handleValueChanged(float temp)
{
    temperature_v->setText(QString::number(temp, 'f', 2));
}

/*
* Member function which handles received fingerprint ID by writing timestamp user login to a log file
*  PARAMETERS : Curent fingerprint ID
*  RETURNS    : NONE
*/
void Values::handleIdChanged(int id)
{

    switch(id) {

       case 0:
         {
             int fd;

             int wbytes;

             char name[200];

             user->setText("Chirayu");

             time_t t = time(NULL);

             struct tm tm = *localtime(&t);
             
             // Create timestamp for user login
             sprintf(name,"USER NAME : Chirayu LOGIN TIME : %d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
             
             // Write timestamp to file
             fd = open("/var/tmp/idlog.txt",O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
             if(fd<0){

                printf("Error in opening file\n");
                
            }
       
            // Write buffer to file
            wbytes = write(fd,name,strlen(name));
            if (wbytes == -1){
        
            printf("Error in writing to file\n");
            
            }

            // Close file descriptor
            ::close(fd);
             
             break;
         } 

         case 1:{


            int fd,wbytes;

            char name[200];

            user->setText("Shrikant");

            time_t t = time(NULL);

            struct tm tm = *localtime(&t);
            
             // Create timestamp for user login
            sprintf(name,"USER NAME : Shrikant LOGIN TIME : %d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            
            // Write timestamp to file
            fd = open("/var/tmp/idlog.txt",O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
            
            if(fd<0){

                printf("Error in opening file\n");
                
            }
       
            // Write buffer to file
            wbytes = write(fd,name,strlen(name));
            if (wbytes == -1){
        
            printf("Error in writing to file\n");
            
            }

            // Close file descriptor
            ::close(fd);
             
             break;
         }

       default : user->setText("Unknown User");

    }

}
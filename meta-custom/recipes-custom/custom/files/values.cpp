#include <QtWidgets>
#include "values.h"

Values::Values()
{
    // Various Labels implemented inside GUI
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *app_name = new QLabel(tr("AESD SECURE ATTENDANCE"));

    QFont s = app_name->font();
    s.setPointSize(88);
    s.setBold(true);
    s.setItalic(true);
    app_name->setFont(s);


    app_name->setAlignment(Qt::AlignLeft);



    QLabel *temperature_l = new QLabel(tr("USER BODY TEMP.(°C)"));


    QFont t = temperature_l->font();
    t.setPointSize(40);
    t.setBold(true);
    temperature_l->setFont(t);


    temperature_l->setAlignment(Qt::AlignHCenter);

    
    temperature_v = new QLabel();


    QFont f = temperature_v->font();
    f.setPointSize(40);
    f.setBold(true);
    temperature_v->setFont(f);
    
    temperature_v->setAlignment(Qt::AlignHCenter);


    QLabel *fingerprint = new QLabel(tr("FINGERPRINT STATUS"));

    QFont u = fingerprint->font();
    u.setPointSize(40);
    u.setBold(true);
    fingerprint->setFont(u);

    fingerprint->setAlignment(Qt::AlignHCenter);


    user = new QLabel();

    QFont v = user->font();
    v.setPointSize(40);
    v.setBold(true);
    user->setFont(v);

    user->setAlignment(Qt::AlignHCenter);
    
    
    layout->addWidget(app_name);
    layout->addWidget(temperature_l);
    layout->addWidget(temperature_v);
    layout->addWidget(fingerprint);
    layout->addWidget(user);


    setLayout(layout);
}

// Member function to update displayed temperature value
void Values::handleValueChanged(float temp)
{
    temperature_v->setText(QString::number(temp, 'f', 2));
}

// Member function to update displayed temperature value
void Values::handleIdChanged(int id)
{

    switch(id) {

       case 0:
         {
             int fd;

             int wbytes;

             //char name[] = "USER NAME : Chirayu\n";
             char name[200];

             user->setText("Chirayu");

             time_t t = time(NULL);

             stuct tm tm = *localtime(&t);

             sprintf(name,"USER NAME : Chirayu LOGIN TIME : %d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

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

             //char name[] = "USER NAME : Shri\n";
            char name[200];

            user->setText("Shrikant");

            time_t t = time(NULL);

             stuct tm tm = *localtime(&t);

             sprintf(name,"USER NAME : Shrikant LOGIN TIME : %d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

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
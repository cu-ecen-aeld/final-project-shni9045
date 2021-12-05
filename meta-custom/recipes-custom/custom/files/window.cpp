/*
* C++ source file containing impplementation for handling main GUI window
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/

#include <QtWidgets>
#include <QMessageBox>

#include "window.h"
#include "values.h"

Window::Window()
{
    values = new Values;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;
    
    // Buttons for door & mode control
    QPushButton *door_button = new QPushButton("OPEN/CLOSE DOOR");
    QPushButton *mode_button = new QPushButton("ENTRY LOG");

    //const QSize BUTTON_SIZE = QSize(100,100);
    
    // Customizing button appearance
    door_button->setStyleSheet("font:bold;background-color:green;font-size: 50px;height: 48px;width: 120px;");
    mode_button->setStyleSheet("font:bold;background-color:blue;font-size: 50px;height: 48px;width: 120px;");
    
    // Attach functions to buttons on being clicked
    connect(door_button,SIGNAL(clicked()),this,SLOT(handle_doorbutton()));
    connect(mode_button,SIGNAL(clicked()),this,SLOT(handle_modebutton()));

    buttons->addWidget(door_button);
    buttons->addWidget(mode_button);

    layout->addWidget(values);
    layout->addLayout(buttons);

    setLayout(layout);

    setWindowTitle(tr("AESD_FINAL_PROJECT"));       // Window Title
}
/* 
* Member function to handle temperature value
* PARAMETERS : Curent temperature value
* RETURNS    : NONE
*
*/
void Window::handleValueChanged(float temp)
{
    values->handleValueChanged(temp);
}

/* 
* Member function to handle fingerprint id
* PARAMETERS : Curent fingerprint id
* RETURNS    : NONE
*
*/
void Window::handleIdChanged(int id)
{
    values->handleIdChanged(id);
}

/* 
* Member function to handle door open/close button by toggling GPIO pins
* PARAMETERS : NONE
* RETURNS    : NONE
*
*/
void Window::handle_doorbutton()
{
    QMessageBox::about(this,"Message","Relay Is Actuated");

    static const char s_values_str[] = "01";

    int LOW = 0;

	char path1[30],path2[30];
	int fd1,fd2;
    
    // GPIO Pin 21
	snprintf(path1, 30, "/sys/class/gpio/gpio%d/value", 21);
	fd1 = open(path1, O_WRONLY);
	if (-1 == fd1) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");

	}

	if (1 != write(fd1, &s_values_str[LOW == 1 ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");

	}

    // GPIO Pin 20
    snprintf(path2, 30, "/sys/class/gpio/gpio%d/value", 20);
	fd2 = open(path2, O_WRONLY);
	if (-1 == fd2) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");

	}

	if (1 != write(fd2, &s_values_str[LOW == 1 ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");

	}

    sleep(1);
    
    // GPIO Pin 21
    if (1 != write(fd1, &s_values_str[LOW == 0 ? 0 : 1], 1)) {
    fprintf(stderr, "Failed to write value!\n");

	}
    
    // GPIO Pin 20
    if (1 != write(fd2, &s_values_str[LOW == 0 ? 0 : 1], 1)) {
    fprintf(stderr, "Failed to write value!\n");
    
    }

	::close(fd1);

    ::close(fd2);

}


/* 
* Member function to handle User Log button by displaying timestamped User login data
* PARAMETERS : NONE
* RETURNS    : NONE
*
*/
void Window::handle_modebutton()
{
    QMessageBox *msgbox = new QMessageBox;

    QFile *file = new QFile ("/var/tmp/idlog.txt");
    
    if (file->open (QIODevice::ReadOnly) == true)
    {
    msgbox->setText (QString (file->readAll ()));
    file->close ();
    }
    msgbox->exec ();

}
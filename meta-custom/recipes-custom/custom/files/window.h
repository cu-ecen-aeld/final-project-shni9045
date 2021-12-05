/*
* Header file for Window class
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/


#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Values;

/*
* Window class definition for handling main GUI window
*
*/

class Window : public QWidget
{
    Q_OBJECT

public slots:
    void handleValueChanged(float temp);                        // Member function to handle temperature value
    
    void handleIdChanged(int id);                               // Member function to handle fingerprint id
    
    void handle_doorbutton();                                   // Member function to handle door open/close button by toggling GPIO pins
    
    void handle_modebutton();                                   // Member function to handle User Log button by displaying timestamped User login data

public:
    Window();

private:
    Values *values;
};

#endif

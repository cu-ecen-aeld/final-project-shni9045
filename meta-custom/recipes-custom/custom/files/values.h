/*
* Header file for Values class
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/


#ifndef VALUES_H
#define VALUES_H

#include <QWidget>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

class QLabel;


/*
* Values Class to handle updated temperature value, update fingerprint ID and other labels in window
*
*/

class Values : public QWidget
{
    Q_OBJECT

public:
    Values();

public slots:

    void handleValueChanged(float temp);                    // Member function to update displayed temperature value
    void handleIdChanged(int id);                           // Member function to handle received ID from fingerprint sensor

private:
    QLabel *temperature_v;
    QLabel *user;

};

#endif /* VALUES_H */

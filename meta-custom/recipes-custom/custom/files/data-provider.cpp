/*
* C++ source file containing impplementation for reading and passing temperature data & fingerprint id from socket to value class
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/

#include <QtCore/QFile>
#include <QDebug>
#include "data-provider.h"


DataProvider::DataProvider()
{
    QObject::connect(&timer, &QTimer::timeout,
		     this, &DataProvider::handleTimer);
    timer.setInterval(750);                     // 1 second interval
    timer.start();
}

// Member funtion to signal data provider class every 1 second & read temperature, fingerprint id

void DataProvider::handleTimer()
{    
    // Read temperature value received over socket
    QFile temp_f("/var/tmp/tempdata.txt");

    if (!temp_f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    float temp = QString(temp_f.readAll()).toDouble();

    qDebug() << "Temperature: " << temp;

    emit valueChanged(temp);                                         // Emit signal for temperature value
    
    // Read fingerprint id received over socket
    QFile temp_i("/var/tmp/iddata.txt");

    if (!temp_i.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int id  = QString(temp_i.readAll()).toDouble();

    qDebug() << "ID : " << id;

    emit idchanged(id);                                             // Emit signal for fingerprint id


}

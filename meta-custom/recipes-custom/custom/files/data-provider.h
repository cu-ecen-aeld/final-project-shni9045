/*
* Header file for DataProvider class
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/

#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <QtCore/QTimer>

/*
* Data Provider class to read temperature sensor data at the rate of 1 Hz
*
*/
class DataProvider: public QObject
{
    Q_OBJECT

public:
    DataProvider();

private slots:
    void handleTimer();               // Member funtion to signal data provider class every 1 second & read temperature, fingerprint id

signals:
    void valueChanged(float temp);    // Signal the received temperature value
    
    void idchanged(int id);           // Signal the received fingerprint value

private:
    QTimer timer;
};

#endif /* DATA_PROVIDER_H */

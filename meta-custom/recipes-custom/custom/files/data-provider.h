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
    void handleTimer();

signals:
    void valueChanged(float temp);

private:
    QTimer timer;
};

#endif /* DATA_PROVIDER_H */

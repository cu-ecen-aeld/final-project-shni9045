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

// Member funtion to signal data provider class every 1 second & read temperature

void DataProvider::handleTimer()
{
    QFile temp_f("/var/tmp/tempdata.txt");

    if (!temp_f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    float temp = QString(temp_f.readAll()).toDouble();

    qDebug() << "Temperature: " << temp;

    emit valueChanged(temp);

    QFile temp_i("/var/tmp/iddata.txt");

    if (!temp_i.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int id  = QString(temp_i.readAll()).toDouble();

    qDebug() << "ID : " << id;

    emit idchanged(id);


}

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
* Values Class to handle updated temperature value & other labels
*
*/

class Values : public QWidget
{
    Q_OBJECT

public:
    Values();

public slots:

    void handleValueChanged(float temp);
    void handleIdChanged(int id);

private:
    QLabel *temperature_v;
    QLabel *user;

};

#endif /* VALUES_H */

#ifndef VALUES_H
#define VALUES_H

#include <QWidget>

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

private:
    QLabel *temperature_v;

};

#endif /* VALUES_H */

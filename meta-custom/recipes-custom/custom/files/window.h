#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Values;

/*
* Window classe definition for handling main GUI window
*
*/

class Window : public QWidget
{
    Q_OBJECT

public slots:
    void handleValueChanged(float temp);

public:
    Window();

private:
    Values *values;
};

#endif

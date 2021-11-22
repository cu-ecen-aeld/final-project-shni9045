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
    void handle_doorbutton();
    void handle_modebutton();

public:
    Window();

private:
    Values *values;
};

#endif

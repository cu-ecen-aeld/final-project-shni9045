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

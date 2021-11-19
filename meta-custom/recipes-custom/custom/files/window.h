#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Values;

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

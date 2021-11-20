#include <QtWidgets>

#include "window.h"
#include "values.h"

Window::Window()
{
    values = new Values;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;

    QPushButton *values_button = new QPushButton("OPEN/CLOSE DOOR");
    QPushButton *chart_button = new QPushButton("AUTOMATIC MODE");

    buttons->addWidget(values_button);
    buttons->addWidget(chart_button);

    layout->addWidget(values);
    layout->addLayout(buttons);

    setLayout(layout);

    setWindowTitle(tr("AESD_FINAL_PROJECT"));
}

void Window::handleValueChanged(float temp)
{
    values->handleValueChanged(temp);
}

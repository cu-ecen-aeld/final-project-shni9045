#include <QtWidgets>

#include "window.h"
#include "values.h"

Window::Window()
{
    values = new Values;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;
    
    // Buttons for door & mode control
    QPushButton *door_button = new QPushButton("OPEN/CLOSE DOOR");
    QPushButton *mode_button = new QPushButton("AUTOMATIC MODE");

    const QSize BUTTON_SIZE = QSize(100,100);
    
    // Customizing button appearance
    door_button->setStyleSheet("font:bold;background-color:green;font-size: 50px;height: 48px;width: 120px;");
    mode_button->setStyleSheet("font:bold;background-color:blue;font-size: 50px;height: 48px;width: 120px;");

    buttons->addWidget(door_button);
    buttons->addWidget(mode_button);

    layout->addWidget(values);
    layout->addLayout(buttons);

    setLayout(layout);

    setWindowTitle(tr("AESD_FINAL_PROJECT"));       // Window Title
}
// Memeber function to handle temperature value
void Window::handleValueChanged(float temp)
{
    values->handleValueChanged(temp);
}

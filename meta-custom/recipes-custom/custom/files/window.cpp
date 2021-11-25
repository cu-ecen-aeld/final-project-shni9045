#include <QtWidgets>
#include <QMessageBox>

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

    //const QSize BUTTON_SIZE = QSize(100,100);
    
    // Customizing button appearance
    door_button->setStyleSheet("font:bold;background-color:green;font-size: 50px;height: 48px;width: 120px;");
    mode_button->setStyleSheet("font:bold;background-color:blue;font-size: 50px;height: 48px;width: 120px;");
    
    // Attach functions to buttons on being clicked
    connect(door_button,SIGNAL(clicked()),this,SLOT(handle_doorbutton()));
    connect(mode_button,SIGNAL(clicked()),this,SLOT(handle_modebutton()));

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

// Memeber function to handle fingerprint id
void Window::handleIdChanged(int id)
{
    values->handleIdChanged(id);
}

void Window::handle_doorbutton()
{
    QMessageBox::about(this,"Message","Relay Is Actuated");

    static const char s_values_str[] = "01";

    int LOW = 0;

    int value = 1;

	char path[30];
	int fd;

	snprintf(path, 30, "/sys/class/gpio/gpio%d/value", 21);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");

	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");

	}

	::close(fd);

}

void Window::handle_modebutton()
{
    QMessageBox::about(this,"Message","Mode Is Changed");
}
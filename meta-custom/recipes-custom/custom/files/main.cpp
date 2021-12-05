/*
* C++ source file containing impplementation calls for GUI 
* Author - @shrikant nimhan shni9045@colorado.edu
*
*/


#include <QApplication>
#include "window.h"
#include "data-provider.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DataProvider dp;
    Window window;

    // Call to connect temperature display slot to handle temperature signal

    QObject::connect(&dp, &DataProvider::valueChanged,
		     &window, &Window::handleValueChanged);
    
    // Call to connect temperature display slot to handle temperature signal

    QObject::connect(&dp, &DataProvider::idchanged,
		     &window, &Window::handleIdChanged);
    
    // Set application window styling
    window.setFixedSize(480, 800);
    window.setStyleSheet("background-color: gray;");
    window.show();
    return app.exec();
}
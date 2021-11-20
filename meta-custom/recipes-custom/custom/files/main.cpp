#include <QApplication>
#include "window.h"
#include "data-provider.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DataProvider dp;
    Window window;

    QObject::connect(&dp, &DataProvider::valueChanged,
		     &window, &Window::handleValueChanged);

    window.setFixedSize(480, 800);
    window.setStyleSheet("background-color: gray;");
    window.show();
    return app.exec();
}
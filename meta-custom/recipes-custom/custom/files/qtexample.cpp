#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );

    QMainWindow mainWindow;

    QLabel *label = new QLabel(&mainWindow);
    label->setText("AESD FINAL PROJECT\@shrikantnimhan");


    QPushButton hello( "Hello world!", &mainWindow);
    hello.resize( 100, 30 );

    mainWindow.show();

    return a.exec();
}
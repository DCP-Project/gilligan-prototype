#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Gilligan (Minnow Client)");
    a.setApplicationVersion("0.1-experimental");

    MainWindow w;
    w.show();

    return a.exec();
}

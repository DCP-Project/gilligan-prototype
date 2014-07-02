#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Gilligan (Minnow Client)");
    a.setApplicationVersion("0.1-experimental");

    // All DCP strings are UTF-8, per spec.
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return a.exec();
}

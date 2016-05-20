#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("DesClient_zh_CN");
    a.installTranslator(&translator);

    MainWindow w;
    w.showFullScreen();
    return a.exec();
}

#include "mainxavierevt.h"
#include <QApplication>

void gpuInfo();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainXavierEvt w;
    gpuInfo();
    w.show();
    return a.exec();
}

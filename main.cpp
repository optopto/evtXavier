#include "mainxavierevt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainXavierEvt w;
    w.show();
    /*cv::Mat imagen = cv::imread( "/home/nvxv/Documents/Proyecto/stream/imagen_1.bmp",0);
    cv::imshow("prueba",imagen);
    cv::waitKey(0);*/
    return a.exec();
}

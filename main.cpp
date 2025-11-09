#include "mainwindow.h"

#include <QApplication>
#include <QMetaType>
#include "product.h"
int main(int argc, char *argv[])
{
    qRegisterMetaType<Product>("Product");
    qRegisterMetaType<Product>("Product&");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

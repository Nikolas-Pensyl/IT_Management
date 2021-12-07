#include "widget.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout<<"test"<<std::endl;
    Widget w;
    w.show();
    return a.exec();
}

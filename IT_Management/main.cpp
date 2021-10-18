#include "startwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //std::cout<<"test";
    StartWidget w;
    w.show();
    return a.exec();
}

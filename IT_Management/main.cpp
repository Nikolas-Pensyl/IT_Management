#include "startwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    int current_exit_code = 0;
    do{
    QApplication a(argc, argv);
    //std::cout<<"test";
    StartWidget w;
    w.show();
    current_exit_code = a.exec();
    } while(current_exit_code==EXIT_CODE_REBOOT);
    return current_exit_code;
}

#include "startwidget.h"

#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <string>


int main(int argc, char *argv[])
{
    int current_exit_code = 0;
    do{
    QApplication a(argc, argv);
    StartWidget w(nullptr,current_exit_code);
    w.show();
    current_exit_code = a.exec();
    } while(current_exit_code==EXIT_CODE_REBOOT_NET);
    return current_exit_code;
}

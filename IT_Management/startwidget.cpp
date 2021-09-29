#include "startwidget.h"
// include OS

//Run winbdoiwremnote.exe
StartWidget::StartWidget(QWidget *parent) : QWidget(parent) {
    QTabWidget* tab;
    tab = new QTabWidget(this);
    IPlist_widget = new QTabWidget;
    IPlist_widget->setStyleSheet("background-color:black;");
    settings_widget = new QTabWidget;
    settings_widget->setStyleSheet("background-color:red;");
    login_widget = new QTabWidget;
    IPlist_widget->setStyleSheet("background-color:blue;");
    tab->addTab(IPlist_widget, "Computers by IP");
    tab->addTab(settings_widget, "Settings");
    tab->addTab(login_widget, "login");
    tab->setFixedSize(700, 500);
}

StartWidget::~StartWidget()
{
}


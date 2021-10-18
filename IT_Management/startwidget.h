#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSignalMapper>

using namespace std;
class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();
    string exec(string command);
    vector<string> ScanLAN();
    vector<QPushButton*> comps;
    vector<QHBoxLayout*> myHLayouts;
    QVBoxLayout *fullLayout;

private:
    QTabWidget* IPlist_widget;
    QTabWidget* login_widget;
    QTabWidget* settings_widget;
    vector<string> LAN;

public slots:
    void execut(QString cmd);
};
#endif // STARTWIDGET_H

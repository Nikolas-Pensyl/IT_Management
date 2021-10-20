#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <QTimer>
#include <sstream>
#include <vector>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QLabel>
#include <fstream>
#include <QLineEdit>
#include <ctime>
#include <QComboBox>

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
    vector<QPushButton*> butt_alls;
    QPushButton *login_button;
    vector<QHBoxLayout*> myHLayouts;
    QStringList commands = {"ping ", "tracert "};
    vector<QVBoxLayout*> total_layout;
    void login();
    void CompsbyIP();

private:
    vector<QTabWidget*> IPlist_widget;
    QTabWidget* login_widget;
    QTabWidget* settings_widget;
    vector<string> LAN;
    bool logged = false;
    QLineEdit *usern;
    QLineEdit *passwo;
    QLabel *label;
     QLabel *label_t;
     QTabWidget* tab;
     vector<QComboBox*> comm;

public slots:
    void execut(QString cmd);
    void runAll(QString cmd);
    void loggerIn();
};
#endif // STARTWIDGET_H

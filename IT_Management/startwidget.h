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
    void settings();

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

     //Settings Widget
     QLineEdit *change_user;
     QLineEdit *change_pass;
     QPushButton *change_login;
     QPushButton *ScanNow;
     QLabel *change_log;
     QLabel *change_use;
     QLabel *change_pas;
     QHBoxLayout *change_u;
     QHBoxLayout *change_p;
     QVBoxLayout *change_full;

     //scanner on settings
     QHBoxLayout *scans_hard;
     QHBoxLayout *scans_soft;
     QHBoxLayout *scans_netw;
     QLabel *scanner;
     QLabel *scan_had;
     QLabel *scan_sot;
     QLabel *scan_net;
     QLabel *minutes_txt;
     QLabel *minutes_tx;
     QLabel *minutes_t;
     QLineEdit *scan_hard;
     QLineEdit *scan_soft;
     QLineEdit *scan_netw;
     QPushButton *scans_had;
     QPushButton *scans_sot;
     QPushButton *scans_net;

     QPushButton *remover_butt;
     QLineEdit *IP_remover;
     QLabel *remover;
     QLabel *rem_name;
     QHBoxLayout *removel;

public slots:
    void execut(QString cmd);
    void runAll(QString cmd);
    void loggerIn();
    void changeLogin();
};
#endif // STARTWIDGET_H

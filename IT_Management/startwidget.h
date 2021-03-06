#ifndef STARTWIDGET_H
#define STARTWIDGET_H

//Any class needed by the StartWidget .cpp file has been included here
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
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QAction>
#include <map>
#include <QInputDialog>
#include <filesystem>
#include <chrono>
#include <thread>

/*
 * This is the number used to indicate a reboot of the program
 * Which can be seen by any .cpp file that includes this .h file
 * */
static int const EXIT_CODE_REBOOT_NET = -99;


using namespace std;

/*
 * Anything inside this instiation of the StartWidget class
 * can only be seen by 'StartWidget', Methods
 * */
class StartWidget : public QWidget
{
    Q_OBJECT

public:
    //These are the methods to be called by an outside class
    StartWidget(QWidget *parent = nullptr, int current_code = 0);
    ~StartWidget();

protected:
    void timerEvent(QTimerEvent *e);

    private:
    /*
     * This is the decleration of some methods
     * for information on a specific method visit
     * this file's corresponding .cpp file
     * and find the method and it's description will be one line
     * above the start of the method.
     * */
    string exec(string command);
    vector<string> ScanLAN();
    bool validateVal(int IP);
    string checkIP(string line);
    string to_IP(QLineEdit *a, QLineEdit *b, QLineEdit *c, QLineEdit *d);
    void reWriteIPs(vector<string> liste, string file_name);

    //'Computers X' widget items
    vector<QPushButton*> comps;
    vector<QPushButton*> butt_alls; 
    vector<QHBoxLayout*> myHLayouts;
    //commands to be shown in the combo box
    QStringList commands = {"ping ", "tracert ", "Transfer File ", "Scan Software ", "Scan Hardware ", "Shutdown "};
    //Full layout of each 'Computers X' widget
    vector<QVBoxLayout*> total_layout;
    int started_code;
    //The login button for the login tab
    QPushButton *login_button;

    //Main three methods for Setup of the UI
    void login();
    void CompsbyIP();
    void settings();

    //Method that tracks the IP and its name when first running this program
    void open_Mapper();

    QFont font;


    vector<QTabWidget*> IPlist_widget;
    vector<string> blackList_IP;
    QTabWidget* login_widget;
    QTabWidget* settings_widget;
    vector<string> LAN;
    vector<string> registeredIPs;
    vector<string> registeredIPs_user;
    vector<string> registeredIPs_pass;


    QLineEdit *usern;
    QLineEdit *passwo;

    QLabel *label;
     QTabWidget* tab;
     vector<QComboBox*> comm;

     //Settings Widget
     //Change username and password part of settings widget
     QLineEdit *change_user;
     QLineEdit *change_pass;
     QPushButton *change_login;
     QPushButton *ScanNow;
     QLabel *change_log;
     QLabel *change_use;
     QLabel *change_pas;
     QHBoxLayout *change_u;
     QHBoxLayout *change_p;
     QVBoxLayout *login_layout;
     QVBoxLayout *full_login_layout;
     QHBoxLayout *login_butto;

     QVBoxLayout *change_full;

     //scanner on settings
     QVBoxLayout *scan_all;
     QHBoxLayout *scans_netw;
     QLabel *scanner;
     QLabel *scan_net;
     QLabel *minutes_t;
     QLineEdit *scan_netw;
     QPushButton *scans_net;

     //removal 'IP' part of the settings widget
     QVBoxLayout *description_text;
     QPushButton *remover_butt;
     QLineEdit *IP_remover_z;
     QLineEdit *IP_remover_o;
     QLineEdit *IP_remover_t;
     QLineEdit *IP_remover_th;
     QLabel *description;
     QLabel *rem_name;
     QLabel *rem_dot_z;
     QLabel *rem_dot_o;
     QLabel *rem_dot_t;
     QHBoxLayout *removel;


     //Give an IP a name
     QVBoxLayout *IP_namer;
     QHBoxLayout *namer;
     QHBoxLayout *namerr;
     QPushButton *namer_button;
     QLineEdit *IP_namer_z;
     QLineEdit *IP_namer_o;
     QLineEdit *IP_namer_t;
     QLineEdit *IP_namer_th;
     QLineEdit *IP_namers;
     QLabel *name_dot_z;
     QLabel *name_dot_o;
     QLabel *name_dot_t;
     QLabel *label_IP;
     QLabel *label_name;
     QLabel *description_name;
     map<string, string> IP_Name;

     //Register an IP
     QVBoxLayout *IP_register;
     QHBoxLayout *regi;
     QPushButton *reg_button;
     QLineEdit *IP_reg_z;
     QLineEdit *IP_reg_o;
     QLineEdit *IP_reg_t;
     QLineEdit *IP_reg_th;
     QLineEdit *IP_reg;
     QLabel *reg_dot_z;
     QLabel *reg_dot_o;
     QLabel *reg_dot_t;
     QLabel *label_reg;
     QLabel *description_reg;

     //manual text entry for an IP
     vector<QVBoxLayout*> IP_ent_full_lay;
     vector<QHBoxLayout*> IP_ent_Hlay;
     vector<QPushButton*> ent_button;
     vector<vector<QLineEdit *>> IP_entry_vect;
     vector<vector<QLabel *>> IP_entry_dot;
     vector<QLabel*> label_ent;
     vector<QLabel*> description_ent;

     //this stores the current username and password
     string password;
     string username;

     //this is the declaration of two private methods
     //for a description of each go to the corresponding .cpp file
     void message(QString str);
     void validateIntervalScan();
     void compare_black_and_regist();

     int network_time=-1;

     //This variable holds when the QTimerEvent method will be called
     //It is started after the user is logged in.
     QTimer *timerID_net;

     //This is the encryption key used for reboots
     const string encrypt = "UYgUYGGUhjjkDDGh&36*&(987979HtyHJreyHFFUtyyyiutiiuh7*&^";

     //'pas' and 'use' are used to detect if the username and password are properly stored
     bool pas=false, use=false;
     //These strings are used for the actual searching of the stored username and password
     string txt, passw, user;
     //Whether the user is logged in or not
     bool logged = false;

     QString Popup();
     QString StringPop();
     QString StringPop1();
     QString StringPop2();
     void SCP(string IP);

     vector<string> pingAll();
     bool isSpecialCharacter(char input);
     string get_ip_from_ipconfig(string full_out);
     void createAHKSoft(string ip_var);
     void createAHKSoft(string ip_var, string server_user, string server_pass);
     void createAHKHard(string ip_var);
     void createAHKHard(string ip_var, string server_user, string server_pass);

    string get_Text_From_User(QString popup_text);
    string get_Text_Password_User(QString popup_text);
    string parsingSpecialCharacters(string in);
    bool isFirstSSH(string ask);

     void ScanSoft();
     void ScanSoft(string ip_var);
     void ScanHard();
     void ScanHard(string ip_var);
     void Shutdown(string ip_var);

     clock_t seconds;


     /*
      * 'slots' are methods that are run based on an action by the user or
      * SIGNALS sent out by the program. For more information on each
      * individual method go to this file's corresponding .cpp file
      * and read the given description
      * */
public slots:
    void execut(QString cmd);
    void runAll(QString cmd);
    void loggerIn();
    void changeLogin();
    void textSetting(int tabbe);
    void changeInterval();
    void reScan();
    void blackList();
    void reName();
    void registerIP();
    void filter(QString cmd);
};
#endif // STARTWIDGET_H

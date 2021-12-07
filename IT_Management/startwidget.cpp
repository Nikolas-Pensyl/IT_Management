#include "startwidget.h"

using namespace std;

/*
 * This method is the constructor of the 'StartWidget' class
 * Which sets up anything that needs set up
 * like the tabs of the program and the proper formating
 * and connecting of objects as well as checking if
 * the encryption is active by checking if 'logged' = true
 * at which the program sends a signal as if the login_button
 * has been pressed and invoking whatever that may
 * */
StartWidget::StartWidget(QWidget *parent, int current_code) : QWidget(parent) {
    //Create tabs

    tab = new QTabWidget(this);
    IPlist_widget.push_back(new QTabWidget);
    settings_widget = new QTabWidget;
    login_widget = new QTabWidget;



    //Set the tabs to the Screen

    login();
    open_Mapper();
    CompsbyIP();
    settings();
    validateIntervalScan();

    tab->addTab(login_widget, "login");
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(textSetting(int)));
    tab->setFixedSize(800, 700);

    hard_soft = hardware_scan==software_scan;
    soft_net = software_scan==network_scan;
    hard_net = hardware_scan==network_scan;

    if(logged) {
        emit login_button->clicked();
        if(current_code==EXIT_CODE_REBOOT_NET) {
            if(soft_net) {
                //Scan Software
            } else {
                timerID_soft->setInterval(software_time);
            }
            if(hard_net) {
                //Scan Hardware
            } else {
                timerID_hard->setInterval(hardware_time);
            }

        } else if(current_code==EXIT_CODE_REBOOT_HARD) {
            if(hard_soft) {
                //Scan Software
            } else {
                timerID_soft->setInterval(software_time);
            }
            if(hard_net) {
                //Scan Network
            } else {
                timerID_hard->setInterval(network_time);
            }

        } else if(current_code==EXIT_CODE_REBOOT_SOFT) {
            if(hard_soft) {
                //Scan Hardware
            } else {
                timerID_hard->setInterval(hardware_time);
            }
            if(soft_net) {
                //Scan Network
            } else {
                timerID_net->setInterval(network_time);
            }
        }
    }
}


/*
 * This method sets the text input fields in the settings tab
 * This is so if a user accidentially clicks the wrong button
 * The user won't have the settings changed without changing
 * the actual input fields
 * */
void StartWidget::textSetting(int tabChosen) {
    if(tab->indexOf(settings_widget)==tabChosen) {
        change_user->setText(QString::fromStdString(username));
        change_pass->setText(QString::fromStdString(password));
        scan_hard->setText(QString::fromStdString(to_string(hardware_scan)));
        scan_soft->setText(QString::fromStdString(to_string(software_scan)));
        scan_netw->setText(QString::fromStdString(to_string(network_scan)));
    }
}

/*
 * When this method is called it restarts the program
 * which triggers a reScan
 * */
void StartWidget::reScan() {
    qDebug() << "Performing reboot0";
    ofstream ofile;
    ofile.open("../loggedin");
    ofile<<encrypt<<endl;
    ofile<<"0"<<endl;
    ofile<<timerID_hard->remainingTime()<<endl;
    ofile<<timerID_soft->remainingTime()<<endl;
    ofile.close();
    qApp->exit(EXIT_CODE_REBOOT_NET);
}

/*
 * When this method is called it restarts the program
 * which triggers a reScan
 * */
void StartWidget::reScanHard() {
    qDebug() << "Performing reboot1";
    ofstream ofile;
    ofile.open("../loggedin");
    ofile<<encrypt<<endl;
    ofile<<timerID_net->remainingTime()<<endl;
    ofile<<"0"<<endl;
    ofile<<timerID_soft->remainingTime()<<endl;
    ofile.close();
    qApp->exit(EXIT_CODE_REBOOT_HARD);
}

/*
 * When this method is called it restarts the program
 * which triggers a reScan
 * */
void StartWidget::reScanSoft() {
    qDebug() << "Performing reboot2";
    ofstream ofile;
    ofile.open("../loggedin");
    ofile<<encrypt<<endl;
    ofile<<timerID_net->remainingTime()<<endl;
    ofile<<timerID_hard->remainingTime()<<endl;
    ofile<<"0"<<endl;
    ofile.close();
    qApp->exit(EXIT_CODE_REBOOT_SOFT);
}

/*
 * This method is the method activated when the change_login button is clicked
 * This rewrites the login information for when the user logs in next
 **/
void StartWidget::changeLogin() {
    username = change_user->text().toStdString();
    password = change_pass->text().toStdString();
    remove("../logininfo.txt");
    ofstream ofile;
    ofile.open("../logininfo.txt");
    ofile<<"username:" << username <<endl;
    ofile<<"password:"<<password<<endl;
    ofile.close();
    message("Successfully changed the login information!!");
}

/*
 * This method is a SLOT (which is a method activated on a SIGNAL)
 * This method takes the given input
 * from the 'Hardware', 'Software' and 'Network' input field
 * and rewrites hong long apart the scans should take place.
**/
void StartWidget::changeInterval() {
    hardware_scan = atoi(scan_hard->text().toStdString().c_str());
    software_scan = atoi(scan_soft->text().toStdString().c_str());
    network_scan = atoi(scan_netw->text().toStdString().c_str());
    remove("../ScanInfo.txt");
    ofstream ofile;
    ofile.open("../ScanInfo.txt");
    ofile<<"Hardware:" << hardware_scan <<endl;
    ofile<<"Software:"<<software_scan<<endl;
    ofile<<"Network:"<<network_scan<<endl;
    ofile.close();
    message("Scan Interval changed successfully!!");
    timerID_hard->setInterval(hardware_scan*60*1000);
    timerID_soft->setInterval(software_scan*60*1000);
    timerID_net->setInterval(network_scan*60*1000);
}

/*
 * This method takes a string as an input
 * and outputs a popup notifying the user of the message
 * */
void StartWidget::message(QString str) {
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

/*
 * This method is called on the start of the program
 * This method is responsible for making the layout of the settings tab
 * */
void StartWidget::settings() {
    QFont font;

    //Change login
    change_log = new QLabel(settings_widget);
    change_use = new QLabel(settings_widget);
    change_pas = new QLabel(settings_widget);
    change_log->setText("Change your login info here");
    change_use->setText("Username: ");
    change_pas->setText("Password:  ");
    change_user = new QLineEdit();
    change_pass = new QLineEdit();
    change_user->setText(QString::fromStdString(username));
    change_pass->setText(QString::fromStdString(password));
    change_login = new QPushButton();
    change_login->setText("Change Login");
    connect(change_login, SIGNAL(clicked()), this, SLOT(changeLogin()));
    change_u = new QHBoxLayout();
    change_p = new QHBoxLayout();
    login_layout = new QVBoxLayout();
    change_full = new QVBoxLayout();

    font = change_log->font();
    font.setPointSize(10);
    settings_widget->setFont(font);

    change_u->addWidget(change_use);
    change_u->addWidget(change_user);
    change_p->addWidget(change_pas);
    change_p->addWidget(change_pass);

    login_layout->addWidget(change_log);
    login_layout->addLayout(change_u);
    login_layout->addLayout(change_p);
    login_layout->addWidget(change_login);

    login_layout->setContentsMargins(0, 0, 0, 9);
    change_full->addLayout(login_layout);

    description_text = new QVBoxLayout();
    remover_butt = new QPushButton();
    IP_remover_z = new QLineEdit();
    IP_remover_o = new QLineEdit();
    IP_remover_t = new QLineEdit();
    IP_remover_th = new QLineEdit();
    description = new QLabel(settings_widget);
    rem_name = new QLabel(settings_widget);
    rem_dot_z = new QLabel(settings_widget);
    rem_dot_o = new QLabel(settings_widget);
    rem_dot_t = new QLabel(settings_widget);
    removel = new QHBoxLayout();
    rem_name->setText("IP Address: ");
    rem_dot_z->setText(".");
    rem_dot_o->setText(".");
    rem_dot_t->setText(".");
    description->setFixedHeight(75);
    description->setText("If you leave a space black and press the button it will put a zero when blacklisting.\nEnter a value between 0 and 255 in each box to blacklist an IP address\nEnter the IP Adress you want ignored on the next scan.\nIf an IP was already blacklisted pressing the button with the associated IP entered will readd the IP on next scan.");
    remover_butt->setText("Press to ignore or add entered IP  ");

    IP_remover_z->setValidator(new QIntValidator(0, 255, this));
    IP_remover_o->setValidator(new QIntValidator(0, 255, this));
    IP_remover_t->setValidator(new QIntValidator(0, 255, this));
    IP_remover_th->setValidator(new QIntValidator(0, 255, this));

    connect(remover_butt, SIGNAL(clicked()), this, SLOT(blackList()));
    removel->addWidget(rem_name);
    removel->addWidget(IP_remover_z);
    removel->addWidget(rem_dot_z);
    removel->addWidget(IP_remover_o);
    removel->addWidget(rem_dot_o);
    removel->addWidget(IP_remover_t);
    removel->addWidget(rem_dot_t);
    removel->addWidget(IP_remover_th);
    removel->addWidget(remover_butt);
    description->setContentsMargins(0, 0, 0, 8);
    removel->setContentsMargins(0, 8, 0, 0);
    description->setFont(font);

    description_text->setContentsMargins(0, 15, 0, 15);

    description_text->addWidget(description);
    description_text->addLayout(removel);

    change_full->addLayout(description_text);


    IP_namer = new QVBoxLayout();
    namer = new QHBoxLayout();
    namerr = new QHBoxLayout();
    label_IP = new QLabel(settings_widget);
    label_name = new QLabel(settings_widget);
    name_dot_z = new QLabel(settings_widget);
    name_dot_o = new QLabel(settings_widget);
    name_dot_t = new QLabel(settings_widget);
    description_name = new QLabel(settings_widget);
    IP_namer_z = new QLineEdit();
    IP_namer_o = new QLineEdit();
    IP_namer_t = new QLineEdit();
    IP_namer_th = new QLineEdit();
    IP_namers = new QLineEdit();
    namer_button = new QPushButton();
    namer_button->setText("  Rename the IP  ");
    name_dot_z->setText(".");
    name_dot_o->setText(".");
    name_dot_t->setText(".");
    label_name->setText("Name:      ");
    label_IP->setText("IP Adress: ");
    description_name->setText("Enter a valid enter IP Address as well as the name you want the IP correspond to.\nThen click the corresponding button and on next can the corresponding IP button\nwill be renamed if it is not blacklisted and if it is scanned.");
    IP_namer_z->setValidator(new QIntValidator(0, 255, this));
    IP_namer_o->setValidator(new QIntValidator(0, 255, this));
    IP_namer_t->setValidator(new QIntValidator(0, 255, this));
    IP_namer_th->setValidator(new QIntValidator(0, 255, this));

    connect(namer_button, SIGNAL(clicked()), this, SLOT(reName()));
    namer->addWidget(label_IP);
    namer->addWidget(IP_namer_z);
    namer->addWidget(name_dot_z);
    namer->addWidget(IP_namer_o);
    namer->addWidget(name_dot_o);
    namer->addWidget(IP_namer_t);
    namer->addWidget(name_dot_t);
    namer->addWidget(IP_namer_th);

    namerr->addWidget(label_name);
    namerr->addWidget(IP_namers);
    namerr->addWidget(namer_button);

    IP_namer->setContentsMargins(0, 20, 0, 20);

    IP_namer->addWidget(description_name);
    IP_namer->addLayout(namer);
    IP_namer->addLayout(namerr);

    change_full->addLayout(IP_namer);

    //Register
    IP_register = new QVBoxLayout();
    regi = new QHBoxLayout();
    label_reg = new QLabel(settings_widget);
    reg_dot_z = new QLabel(settings_widget);
    reg_dot_t = new QLabel(settings_widget);
    reg_dot_o = new QLabel(settings_widget);
    description_reg = new QLabel(settings_widget);
    IP_reg_z = new QLineEdit();
    IP_reg_o = new QLineEdit();
    IP_reg_t = new QLineEdit();
    IP_reg_th = new QLineEdit();
    IP_reg = new QLineEdit();
    reg_button = new QPushButton();
    reg_button->setText("  Register the IP  ");
    reg_dot_z->setText(".");
    reg_dot_o->setText(".");
    reg_dot_t->setText(".");
    label_reg->setText("IP Adress: ");
    description_reg->setText("Enter a valid enter IP Address and once the button is clicked the IP will be registered\nmeaning anytime you start up the software you will be notified if a corresponding IP is offline.");
    IP_reg_z->setValidator(new QIntValidator(0, 255, this));
    IP_reg_o->setValidator(new QIntValidator(0, 255, this));
    IP_reg_t->setValidator(new QIntValidator(0, 255, this));
    IP_reg_th->setValidator(new QIntValidator(0, 255, this));

    connect(reg_button, SIGNAL(clicked()), this, SLOT(registerIP()));
    regi->addWidget(label_reg);
    regi->addWidget(IP_reg_z);
    regi->addWidget(reg_dot_z);
    regi->addWidget(IP_reg_o);
    regi->addWidget(reg_dot_o);
    regi->addWidget(IP_reg_t);
    regi->addWidget(reg_dot_t);
    regi->addWidget(IP_reg_th);
    regi->addWidget(reg_button);

    IP_register->setContentsMargins(0, 20, 0, 20);

    IP_register->addWidget(description_reg);
    IP_register->addLayout(regi);

    change_full->addLayout(IP_register);


    scan_all = new QVBoxLayout();
    scans_hard = new QHBoxLayout();
    scans_netw = new QHBoxLayout();
    scans_soft = new QHBoxLayout();
    scan_hard = new QLineEdit();
    scan_netw = new QLineEdit();
    scan_soft = new QLineEdit();
    scan_hard->setValidator(new QIntValidator(10, 1000, this));
    scan_netw->setValidator(new QIntValidator(10, 1000, this));
    scan_soft->setValidator(new QIntValidator(10, 1000, this));
    scanner = new QLabel(settings_widget);
    scan_had = new QLabel(settings_widget);
    scan_sot = new QLabel(settings_widget);
    scan_net = new QLabel(settings_widget);
    minutes_txt = new QLabel(settings_widget);
    minutes_tx = new QLabel(settings_widget);
    minutes_t = new QLabel(settings_widget);
    scanner->setFixedHeight(15);
    scanner->setText("Set the interval at which each scan occurs");
    scan_had->setText("Scan Hardware Interval");
    scan_net->setText("Scan Network Interval ");
    scan_sot->setText("Scan Software Interval");
    minutes_txt->setText("minutes");
    minutes_tx->setText("minutes");
    minutes_t->setText("minutes");

    scans_sot = new QPushButton();
    scans_had = new QPushButton();
    scans_net = new QPushButton();

    scans_had->setText("Set Hardware Scan Interval ");
    scans_sot->setText("Set Software Scan Interval  ");
    scans_net->setText("Set Network Scan Interval   ");

    connect(scans_sot, SIGNAL(clicked()), this, SLOT(changeInterval()));
    connect(scans_had, SIGNAL(clicked()), this, SLOT(changeInterval()));
    connect(scans_net, SIGNAL(clicked()), this, SLOT(changeInterval()));

    scans_hard->addWidget(scan_had);
    scans_hard->addWidget(scan_hard);
    scans_hard->addWidget(minutes_txt);
    scans_hard->addWidget(scans_had);


    scans_soft->addWidget(scan_sot);
    scans_soft->addWidget(scan_soft);
    scans_soft->addWidget(minutes_tx);
    scans_soft->addWidget(scans_sot);

    scans_netw->addWidget(scan_net);
    scans_netw->addWidget(scan_netw);
    scans_netw->addWidget(minutes_t);
    scans_netw->addWidget(scans_net);


    scanner->setFont(font);

    scan_all->setContentsMargins(0, 30, 0, 0);
    scanner->setContentsMargins(0, 15, 0, 0);
    scan_all->addWidget(scanner);
    scan_all->addLayout(scans_hard);
    scan_all->addLayout(scans_netw);
    scan_all->addLayout(scans_soft);

    change_full->addLayout(scan_all);

    ScanNow = new QPushButton();
    ScanNow->setText("Scan Now");
    connect(ScanNow, SIGNAL(clicked()), this, SLOT(reScan()));

    change_full->addWidget(ScanNow);

    settings_widget->setLayout(change_full);
}

/*
 * This is the deletion method
 * it is responsible for deleting this class
 * which as of right now is not needed as this is the main class
 * But it is kept for furture possible implementation
 * */
StartWidget::~StartWidget()
{
}

/*
 * This is the method/SLOT called when the register IP button is clicked
 * It checks if the IP is valid then it checks to see if it is a duplicate
 * If it is a duplicate it removes the IP from the list otherwise
 * the IP is compared to the Blacklist and if it is not on the blacklist
 * the IP is added otherwise a prompt comes up asking if you want the IP
 * to belong to the blackList, Registered list or neither as it cannont
 * be in both. and then it removes it from the corresponding list[s]
 */
void StartWidget::registerIP() {
    string iper = to_IP(IP_reg_z, IP_reg_o, IP_reg_t, IP_reg_th);

    if(iper.compare("")!=0) {
        int s = 0;
        int i = -1;
        for(vector<string>::iterator IP_REG = registeredIPs.begin(); IP_REG!=registeredIPs.end(); IP_REG++, s++) {
            if(iper.compare(*IP_REG)==0) {
                i = s;
            }

        }
        if(i==-1) {
            bool duo = false;
            for(int blackIP = 0; blackList_IP.size()>blackIP; blackIP++) {
            if(blackList_IP[blackIP].compare(iper)==0) {
                duo = true;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(iper + " is in both the registered and black listed list which list would you like it in: "));
                QAbstractButton* None = msgBox.addButton(tr("None"), QMessageBox::YesRole);
                QAbstractButton* re = msgBox.addButton(tr("Registered list"), QMessageBox::YesRole);
                QAbstractButton* bl = msgBox.addButton(tr("Black list"), QMessageBox::YesRole);
                do {
                    msgBox.exec();
                } while (msgBox.clickedButton()!= None && msgBox.clickedButton()!= re && msgBox.clickedButton()!= bl);
                if(msgBox.clickedButton()==None) {
                    blackList_IP.erase(blackList_IP.begin()+blackIP);
                    blackIP--;
                    reWriteIPs(blackList_IP, "../blackList.txt");
                } else if(msgBox.clickedButton()==re) {
                    registeredIPs.push_back(iper);
                    blackList_IP.erase(blackList_IP.begin()+blackIP);
                    blackIP--;
                    reWriteIPs(blackList_IP, "../blackList.txt");
                    reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
                    message("IP successfully added to registered list!!");       
                }
            }
        }
          if(!duo) {
              registeredIPs.push_back(iper);
              reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
              message("IP successfully added to registered list!!");
          }
        } else {
             registeredIPs.erase(registeredIPs.begin()+i);
             reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
            message("IP successfully removed from registered list!!");
        }
    } else {
        message("Invalid IP try agian!!");
    }
}

/*
 * This method/SLOT is called when the change name button is pressed
 * first it checks if anything is in the name category. If is not it prompts you to
 * try agian with a valid entry. If it is then it checks for a valid IP if
 * it is not a valid IP it prompts you to enter a valid entry.
 * If it is a valid entry then the name is saved to both a mapper in memory and a
 * text file in storage for use on future start ups.
 */
void StartWidget::reName() {
    string iper = to_IP(IP_namer_z, IP_namer_o, IP_namer_t, IP_namer_th);

    if(IP_namers->text().toStdString().compare("")!=0) {
        if(iper.compare("")!=0) {
                IP_Name[iper] = IP_namers->text().toStdString();
            ofstream ofile;
            ofile.open("../IP_Names.txt");
            for(map<string, string>::iterator IP_NAMES = IP_Name.begin(); IP_NAMES!=IP_Name.end(); IP_NAMES++) {
                ofile<<IP_NAMES->first<<","<<IP_NAMES->second<<endl;
                cout<<IP_NAMES->first;
            }
            ofile.close();

            message("IP name successfully added/changed!!");

        }
    } else {
        message("No name entered try Again!!");
    }
}

/*
 * This method takes four entries which if valid is an integer with
 * a value greater or equal to 0 and less than 256.
 * Then if it is Valid it returns the IP as one continuous string
 * Otherwise it returns ""
 */
string StartWidget::to_IP(QLineEdit *a, QLineEdit *b, QLineEdit *c, QLineEdit *d) {
    string iper = "";
    int IP_z = atoi(a->text().toStdString().c_str());
    int IP_o = atoi(b->text().toStdString().c_str());
    int IP_t = atoi(c->text().toStdString().c_str());
    int IP_th = atoi(d->text().toStdString().c_str());
    if(validateVal(IP_z) && validateVal(IP_o) && validateVal(IP_t) && validateVal(IP_th)) {
        iper = to_string(IP_z) + "." + to_string(IP_o) +"."+ to_string(IP_t) +"."+ to_string(IP_th);
    } else {
        message("Invalid IP. Check the address and try agian.");
    }
    return iper;
}

void StartWidget::reWriteIPs(vector<string> liste, string file_name) {
    ofstream ofile;
    ofile.open(file_name);
    for(int IPss = 0; liste.size()>IPss; IPss++) {
        ofile<<liste[IPss]<<endl;
    }
    ofile.close();
}

/*
 * This is the method/SLOT called when the blacklist IP button is clicked
 * It checks if the IP is valid then it checks to see if it is a duplicate
 * If it is a duplicate it removes the IP from the list otherwise
 * the IP is compared to the registered list and if it is not on the registered
 * list, the IP is added otherwise a prompt comes up asking if you want the IP
 * to belong to the blackList, Registered list or neither as it cannont
 * be in both. and then it removes it from the corresponding list[s]
 */
void StartWidget::blackList() {
    string iper = to_IP(IP_remover_z, IP_remover_o, IP_remover_t, IP_remover_th);

    int copy = -1;
    int i = 0;
    if(iper.compare("")!=0) {
        for(vector<string>::iterator IPss = blackList_IP.begin(); IPss!=blackList_IP.end(); IPss++, i++) {
            if(iper.compare(*IPss)==0) {
                copy = i;
            }
        }
        if(copy==-1) {

            bool duo = false;
            for(int blackIP = 0; registeredIPs.size()>blackIP; blackIP++) {
            if(registeredIPs[blackIP].compare(iper)==0) {
                duo = true;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(iper + " is in both the registered and black listed list which list would you like it in: "));
                QAbstractButton* None = msgBox.addButton(tr("None"), QMessageBox::YesRole);
                QAbstractButton* re = msgBox.addButton(tr("Registered list"), QMessageBox::YesRole);
                QAbstractButton* bl = msgBox.addButton(tr("Black list"), QMessageBox::YesRole);
                do {
                    msgBox.exec();
                } while (msgBox.clickedButton()!= None && msgBox.clickedButton()!= re && msgBox.clickedButton()!= bl);
                if(msgBox.clickedButton()==None) {
                    registeredIPs.erase(registeredIPs.begin()+blackIP);
                    blackIP--;
                    reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
                } else if(msgBox.clickedButton()==bl) {
                    blackList_IP.push_back(iper);
                    registeredIPs.erase(registeredIPs.begin()+blackIP);
                    blackIP--;
                    reWriteIPs(blackList_IP, "../blackList.txt");
                    reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
                    message("IP successfully added to black list!!");
                }
            }
        }
            if(!duo) {
                blackList_IP.push_back(iper);
                reWriteIPs(blackList_IP, "../blackList.txt");
                message("IP successfully added to black list!!");
            }
        } else {
            blackList_IP.erase(blackList_IP.begin()+copy);
            reWriteIPs(blackList_IP, "../blackList.txt");
            message("IP successfully removed from black List!!");
        }
    } else {
        message("Invalid IP try agian!!");
    }
}

/*
 * Checks if a value is greater than or equal to 0 and if it is less than or
 * equal to 255 and returns the result.
 */
bool StartWidget::validateVal(int IP) {
    return IP>=0 && IP<=255;
}


/*
 * This method is called at the start of the program
 * it is responsible for all the 'Computer X' tabs
 * It creates thirty 'IP' buttons per tab in case there are more than 30
 * And it creates 6 rows with 5 buttons in each row
 * every button starts with the text of the IP and can be
 * changed to the name of a computer
 * */
void StartWidget::CompsbyIP() {
    //Scan LAN
    LAN = ScanLAN();
    //Set buttons for IPs scanned on LAN
    myHLayouts.push_back(new QHBoxLayout);
    total_layout.push_back(new QVBoxLayout());
     QSignalMapper* signalMapper = new QSignalMapper (this) ;
    int k = 0, tabb = 0, numm = 0;
    for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++, k++) {
        if(k%30==0) {
          if(k!=0) {
              total_layout[tabb]->addLayout(myHLayouts[numm]);
              numm ++;
              myHLayouts.push_back(new QHBoxLayout);
             tabb++;
             IPlist_widget.push_back(new QTabWidget);
             total_layout.push_back(new QVBoxLayout);
          }
          QHBoxLayout *labs = new QHBoxLayout();
          QLabel *label = new QLabel(IPlist_widget[tabb]);
          label->setText("When a button is clicked, run command this command: ");
          labs->addWidget(label);
          labs->addSpacing(10);
           comm.push_back(new QComboBox(IPlist_widget[tabb]));
          comm[tabb]->addItems(commands);
          labs->addWidget(comm[tabb]);
          labs->addSpacing(10);
          QPushButton *butt_all = new QPushButton();
          butt_all->setText("Run command for all");
          labs->addWidget(butt_all);
          QSignalMapper* sigMapper = new QSignalMapper (this) ;
          connect(butt_all, SIGNAL(clicked()), sigMapper, SLOT(map()));
          sigMapper->setMapping(butt_all, comm[tabb]->currentText());
          connect(sigMapper, SIGNAL(mapped(QString)), this, SLOT(runAll(QString)));
          total_layout[tabb]->addLayout(labs);
        } else if(k%5==0 && k!=0) {
            total_layout[tabb]->addLayout(myHLayouts[numm]);
            numm ++;
            myHLayouts.push_back(new QHBoxLayout);
        }
        QPushButton  *labelbed;
        if(IP_Name.find(lss->substr(2, lss->length()))!=IP_Name.end()) {
            labelbed = new QPushButton(QString::fromStdString(IP_Name[lss->substr(2, lss->length())]), IPlist_widget[tabb]);
        } else {
          labelbed =new QPushButton(QString::fromStdString(*lss), IPlist_widget[tabb]);
        }
        comps.push_back(labelbed);
        connect(comps[k],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(comps[k], QString::fromStdString(*lss));


        myHLayouts[numm]->addWidget(comps[k]);
    }
    connect (signalMapper, SIGNAL(mapped(QString)), this, SLOT(execut(QString))) ;
    total_layout[tabb]->addLayout(myHLayouts[numm]);
    for(int i = 0; i!=tabb+1; i++) {
            IPlist_widget[i]->setLayout(total_layout[i]);
    }

}


/*
 * This method is run when any of the 'Run all' buttons is pressed
 * on any of the 'Computers X' tabs
 * After which this method reads the current command in the given box
 * which the program then runs the command through any computer
 * that was previously detected on the Network
 * **/
void StartWidget::runAll(QString cmd) {
    string com = comm[tab->currentIndex()]->currentText().toStdString();
    if(com.compare(commands[2].toStdString())!=0) {
     for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++) {

         cout<<exec(com + *lss);
     }
    } else {
        message("Invalid command for run all pick a unique IP for this command or pick a differnt command to run for all");
    }
}


/*
 * This method is Responsible
 * for the UI of the login tab
 * This method is also responsible for detecting the proper credentials
 * needed to login, but if the program was rebooted from within
 * The program creates an encryption key that is then saved and called
 * upon in this file after which if matched
 * automatically logs the user in and removes the
 * encryption key for security reasons otherwise
 * this method looks at the required username and password
 * and saves it for later to check agianst when the user presses
 * the login button
 * */
void StartWidget::login() {
    usern = new QLineEdit();
    passwo = new QLineEdit();
    label = new QLabel(login_widget);
    QVBoxLayout *login_layout = new QVBoxLayout();
    full_login_layout = new QVBoxLayout();
    QHBoxLayout *User_lay = new QHBoxLayout();
    QHBoxLayout *Pass_lay = new QHBoxLayout();
    login_butto = new QHBoxLayout();
    QLabel *user_label = new QLabel(login_widget);
    QLabel *pass_label = new QLabel(login_widget);
    login_button = new QPushButton("Login", login_widget);
    label->setText("Login in with the details you previously used.\nIf you are new set a user name and password by typing credentials then click login");
    user_label->setText("Username:");
    pass_label->setText("Password:");

    font = user_label->font();
    font.setPointSize(15);
    login_widget->setFont(font);
    font.setPointSize(10);
    setFont(font);

    User_lay->addWidget(user_label);
    User_lay->addSpacing(10);
    User_lay->addWidget(usern);

    Pass_lay->addWidget(pass_label);
    Pass_lay->addSpacing(10);
    Pass_lay->addWidget(passwo);

    login_button->setFixedWidth(350);

    login_butto->addWidget(login_button);
    login_butto->setContentsMargins(225, 10, 225, 0);

    label->setContentsMargins(0, 10, 0, 0);
    User_lay->setContentsMargins(0, 0, 0, 0);
    Pass_lay->setContentsMargins(0, 0, 0, 0);
    login_layout->addWidget(label);
    login_layout->addLayout(User_lay);
    login_layout->addLayout(Pass_lay);
    login_layout->addLayout(login_butto);
    connect(login_button, SIGNAL(clicked()), this, SLOT(loggerIn()));

    full_login_layout->addLayout(login_layout);
    full_login_layout->setContentsMargins(11, 10, 11, 480);
    login_widget->setLayout(full_login_layout);

    ifstream ifile, file;

    file.open("../logininfo.txt");

    while(getline(file, txt)) {
        if(txt.find("username:")!=string::npos) {
            user = txt.substr(9, txt.length());
            use = true;
        } else if(txt.find("password:")!=string::npos) {
            passw = txt.substr(9, txt.length());
            pas = true;
        }
    }

    file.close();

    ifile.open("../loggedin");
    int i =0;
    while(getline(ifile, txt)) {
        if(txt.compare(encrypt)==0) {
            logged = true;
            if(use && pas) {

                username = user;
                password = passw;
            } else {
                username = "Username";
                password = "Password";
            }

        }
        if(i==1) {
            network_time = atoi(txt.c_str());
        } else if (i==2) {
            hardware_time = atoi(txt.c_str());
        } else if(i==3) {
            software_time = atoi(txt.c_str());
        }
        i++;
    }
    ifile.close();
    remove("../loggedin");
}

/*
 * This method is called automatically at the start of the
 * program and scans the proper file
 * to find the scanning interval settings the user desired
 * if the file is not properly located or
 * parts of the file are missing
 * the program will reset the interval scan
 * to the default 30 min
 * */
void StartWidget::validateIntervalScan() {
    bool had = false, sof = false, net = false;
    string txt, H, S, N;
    ifstream file("../ScanInfo.txt");
    while(getline(file, txt)) {
        if(txt.find("Network:")!=string::npos) {
            N = txt.substr(8, txt.length());
            net = true;
        } else if(txt.find("Hardware:")!=string::npos) {
            H = txt.substr(9, txt.length());
            had = true;
        } else if(txt.find("Software:")!=string::npos) {
            S = txt.substr(9, txt.length());
            sof = true;
        }
    }
    if(had && sof && net) {
        hardware_scan = atoi(H.c_str());
        software_scan = atoi(S.c_str());
        network_scan = atoi(N.c_str());

    } else {
        hardware_scan = 30;
        software_scan = 30;
        network_scan = 30;
        remove("../ScanInfo.txt");
        ofstream ofile;
        ofile.open("../ScanInfo.txt");
        ofile<<"Hardware:" << hardware_scan <<endl;
        ofile<<"Software:"<<software_scan<<endl;
        ofile<<"Network:"<<network_scan<<endl;
        ofile.close();
    }

}

/*
 * This is the method called when 1 of two things occur:
 * 1) if the user presses the login button the
 * information entered is compared with the credentials read in earlier
 * if they match the user is logged in otherwise the user
 * is told the credentials are incorrect and must try agian
 * 2) If the program is told to reboot an encryption key is made
 * and after intialization the user is automatically logged in.
 *
 * After the user is logged in from either way the UI is remade to
 * remove the login page and display the 'Settings and 'Computers X' tabs
 * */
void StartWidget::loggerIn() {
    if(pas && use && !logged) {
        if(usern->text().toStdString().compare(user)==0) {
            if(passwo->text().toStdString().compare(passw)==0) {
                logged = true;
                username = usern->text().toStdString();
                password = passwo->text().toStdString();

            }
        }
        if(!logged) {
            message("Invalid Credentials try agian");
        }
    } else if (!logged && (!pas || !use)){
        ofstream ofile;
        ofile.open("../logininfo.txt");
        ofile<<"username:"<<usern->text().toStdString()<<endl;
        ofile<<"password:"<<passwo->text().toStdString();
        ofile.close();
        logged = true;
    }
    if(logged) {
        int i =1;

        compare_black_and_regist();

          if(registeredIPs.size()>0) {
            for(vector<string>::iterator registIP = registeredIPs.begin(); registeredIPs.end()!=registIP; registIP++) {
                bool pinged = false;
            for(vector<string>::iterator ls = LAN.begin(); ls!=LAN.end(); ls++) {
                    if(ls->substr(2).compare(*registIP)==0) {
                        pinged = true;
                        break;
                    }
                }
                if(!pinged) {
                    message(QString::fromStdString(*registIP + " was not found on the network"));
                }
            }
          }
        for(vector<QTabWidget*>::iterator tabber = IPlist_widget.begin(); tabber != IPlist_widget.end(); tabber++, i++) {
            string nam = to_string(i);
            QString name = "Computers " + QString::fromStdString(nam);
            tab->addTab(*tabber, name);
        }
        tab->addTab(settings_widget, "Settings");
        tab->removeTab(0);
        repaint();
        timerID_hard = new QTimer(this);
        connect(timerID_hard, SIGNAL(timeout()), this, SLOT(reScanHard()));
        timerID_hard->start(hardware_scan*60*1000);

        timerID_soft = new QTimer(this);
        connect(timerID_soft, SIGNAL(timeout()), this, SLOT(reScan()));
        timerID_soft->start(software_scan*60*1000);

        timerID_net = new QTimer(this);
        connect(timerID_net, SIGNAL(timeout()), this, SLOT(reScan()));
        timerID_net->start(network_scan*60*1000);
    }

}

/*
 * In case of a timeing event this method is called
 * at which point nothing happens
 */
void StartWidget::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
}

/*
 * This method is called when an individual button on a
 * 'Computers X' tab is pressed. The method takes in
 * the Computer IP' button that was pressed,
 * Then it looks at the active command in the Combo Box
 * at which point the method runs the specified command
 * on the specified computer.
 * */
void StartWidget::execut(QString cmd) {
    string com = cmd.toStdString();
    string commer =comm[tab->currentIndex()]->currentText().toStdString();
    //if commer == File Transfer then cory's code
    //othetrtwise

    if(commer.compare(commands[2].toStdString())==0) {
        SCP(cmd);
    } else {
    cout << exec(commer+com);
    }
}

/*
 * This command takes a full command and sends it to the command
 * line. If you want multiple commands put '&&' inbetween each command
 * to get the same command prompt as after this method ends.
 * The corresponding command line is closed and if called agian
 * the method opens a new command line to utilize.
 * This method also returns the output to where it was called.
 * The last thing this method does is provide a live output to
 * a text only pane as well as provide the output to a text file
 * with a time log to know what went wrong if something went wrong.
 * */
string StartWidget::exec(string command) {
    char buffer[128];
    string result = "";

    // Open pipe to file
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
       return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

       // use buffer to read and add to result
       if (fgets(buffer, 128, pipe) != NULL)
          result += buffer;
    }

    _pclose(pipe);

    time_t now = time(0);
    char* dt = ctime(&now);

    ofstream ofile;
    ofile.open("../Outlog.txt", std::ios_base::app);

    ofile<<dt<<result<<endl<<endl<<endl;
    ofile.close();

    ofstream ifile;
    ifile.open("../Inlog.txt", std::ios_base::app);
    ifile<<dt<<command<<endl<<endl<<endl;
    ifile.close();
    return result;
 }

/*
 * This method Scans the actual network
 * to detect what computers are connected to
 * the computers network and then will first
 * parse the output to seperate each individual computer
 * after which each non-duplicate IP will outputed to the
 * live text pane as well as sent back to wherever
 * this method was called in the format of a vector<string>
 * */
vector<string> StartWidget::ScanLAN() {
    //the '&&' runs multiple commands
   string ts = exec("arp -a");

   cout << ts;

   //Gets the black Listed IPs from the txt file
   ifstream ifile;
   ifile.open("../blackList.txt");
   bool duo = false;
   string line, tested;
       while(getline(ifile, line)) {
           tested = checkIP(line);

           duo = false;
           if(tested.compare("")!=0) {
               for(int i = 0; i<blackList_IP.size(); i++) {
                   if(blackList_IP[i].compare(tested)==0) {
                       duo = true;
                       break;
                   }
               }
               if(!duo) {
                 blackList_IP.push_back(tested);
               }
           }
       }
       ifile.close();

    ifstream ifile_reg;
   ifile_reg.open("../RegisteredIPs.txt");
        while(getline(ifile_reg, line)) {
            tested = checkIP(line);
            if(tested.compare("")!=0) {
                for(int i = 0; i<registeredIPs.size(); i++) {
                    if(registeredIPs[i].compare(tested)==0) {
                        duo = true;
                        break;
                    }
                }
                if(!duo) {
                  registeredIPs.push_back(tested);
                }
            }

        }
   ifile_reg.close();

   //This parses the output string to only get the IPs of the output
   std::vector<string> ls;
   std::istringstream f(ts);
   int current = 0;
      while (std::getline(f, line)) {
          current = 0;
          for(int i =0; i<line.length(); i++) {
              if(line.compare(0, 9, "Interface")==0) {
                  break;
              }
              if(line.compare(i, 1, ".")==0) {
                  current++;
             }
             if(current==3 && line.compare(i, 1, " ")==0) {
                 bool dou = false;
                 for(vector<string>::iterator k = ls.begin(); k!=ls.end(); k++) {
                    if(k->compare(line.substr(0, i))==0) {
                        dou  = true;
                    }
                 }
                 for(vector<string>::iterator l = blackList_IP.begin(); l!=blackList_IP.end() && !dou; l++) {
                     string test = l->substr(0, l->length()) +"  ";
                     if(test.compare(line.substr(2, i))==0) {
                         dou = true;
                     }
                 }
                 if(!dou) {
                     ls.push_back(line.substr(0, i));
                     cout << line.substr(0, i) <<endl;
                    }
                 current = 0;
             }

          }

      }
    return ls;
}

/*
 * This method takes in a string and tests
 * if it is a complete and actual IP address
 * if it is not then we return ""
 * otherwise we return the IP address
 */
string StartWidget::checkIP(string line) {
    int current = 0;
    int count = 0;
    for(int i = 0; i<line.length(); i++) {
        if(count>3) {
            return "";
        } else {
            if(line.compare(i, 1, ".")==0 && count>0) {
                current++;
                if(atoi(line.substr(i-count, i).c_str())>255) {
                    return "";
                }
                count = 0;
            } else if(isdigit(line[i])) {
                count++;
            } else {
                return "";
            }

            if(count==3 && current==3) {
                if(atoi(line.substr(i-count, i).c_str())>255) {

                    return "";
                }
                return line.substr(0, i+1);
            } else if(current==3 && count>0 && !isdigit(line[i+1])){

                if(atoi(line.substr(i-count, i).c_str())>255) {
                    return "";
                }
                return line.substr(0, i+1);
            }
        }
    }

}

/*
 * This method writes all the names to the corresponding IP
 * to a txt file for later use when being reopened
 */
void StartWidget::open_Mapper() {
    ifstream ifile;
    ifile.open("../IP_Names.txt");
    string line, ip, name;
    while(getline(ifile, line)) {
        int current = 0;
        for(int i =0; i<line.length(); i++) {
            if(line.compare(i, 1, ".")==0) {
                current++;
           }
           if(current==3 && line.compare(i, 1, ",")==0) {
               IP_Name[line.substr(0, i)] = line.substr(i+1);
               current = 0;
           }

        }
    }

    cout<<IP_Name.size()<<endl;
    ifile.close();
}

/*
 * This method is called on startup and reads the black list and
 * registered list IPs and compares the two lists.
 * If a match is found it asks which list should keep the IP
 * the options are neither registered or blacklist.
 * and then it rewrites both files as to better preserve the state of the
 * program.
 */
void StartWidget::compare_black_and_regist() {
    for(int registIP = 0; registeredIPs.size()>registIP; registIP++) {
        for(int blackIP = 0; blackList_IP.size()>blackIP; blackIP++) {
        if(blackList_IP[blackIP].compare(registeredIPs[registIP])==0) {
            QMessageBox msgBox;
            msgBox.setText(QString::fromStdString(blackList_IP[blackIP] + " is in both the registered and black listed list which list would you like it in: "));
            QAbstractButton* None = msgBox.addButton(tr("None"), QMessageBox::YesRole);
            QAbstractButton* re = msgBox.addButton(tr("Registered list"), QMessageBox::YesRole);
            QAbstractButton* bl = msgBox.addButton(tr("Black list"), QMessageBox::YesRole);
            do {
                msgBox.exec();
            } while (msgBox.clickedButton()!= None && msgBox.clickedButton()!= re && msgBox.clickedButton()!= bl);
            if(msgBox.clickedButton()==None) {
                blackList_IP.erase(blackList_IP.begin()+blackIP);
                registeredIPs.erase(registeredIPs.begin()+registIP);
                registIP--;
                blackIP--;
            } else if(msgBox.clickedButton()==re) {
                blackList_IP.erase(blackList_IP.begin()+blackIP);
                blackIP--;
            } else {
                registeredIPs.erase(registeredIPs.begin()+registIP);
                registIP--;
            }
        }
    }
    }
    reWriteIPs(registeredIPs, "../RegisteredIPs.txt");
    reWriteIPs(blackList_IP, "../blackList.txt");
}










QString StartWidget::Popup() {
    QMessageBox msgBox;
                msgBox.setText(QString::fromStdString("To what location would you like to send your file?"));
                QAbstractButton* Desktop = msgBox.addButton(tr("Desktop"), QMessageBox::YesRole);
                QAbstractButton* Documents = msgBox.addButton(tr("Documents"), QMessageBox::YesRole);
                QAbstractButton* Downloads = msgBox.addButton(tr("Downloads"), QMessageBox::YesRole);
                do {
                    msgBox.exec();
                } while (msgBox.clickedButton()!= Desktop && msgBox.clickedButton()!= Documents && msgBox.clickedButton()!= Downloads);
                if(msgBox.clickedButton()==Desktop) {
                    QString Desktop= "/Desktop";
                    return Desktop;
                } else if(msgBox.clickedButton()==Documents) {
                    QString Documents= "/Documents";
                    return Documents;
                } else if(msgBox.clickedButton()==Downloads){
                    QString Downloads= "/Downloads";
                    return Downloads;
                }
                else{
                    return NULL;
                }
}

QString StartWidget::StringPop() {
    bool ok;
    QString location;
    QString blank = "";
    QString text = QInputDialog::getText(this, tr("Location"), tr("Please enter the file location of what file you wish to transfer:"), QLineEdit::Normal, blank, &ok);

    location=text;
    location.toStdString();
return location;
}

QString StartWidget::StringPop1() {
    bool ok;
    QString TargetUser;
    QString blank1 ="";
    QString text1 = QInputDialog::getText(this, tr("Target User"), tr("Please enter the target User you which to transfer to:"), QLineEdit::Normal, blank1, &ok);

    TargetUser=text1;
    TargetUser.toStdString();
return TargetUser;
}

QString StartWidget::StringPop2() {
    bool ok;
    QString IP;
    QString blank1 ="";
    QString text1 = QInputDialog::getText(this, tr("IP"), tr("Please enter the IP address you which to transfer to:"), QLineEdit::Normal, blank1, &ok);

    IP=text1;
    IP.toStdString();
    return IP;
}

void StartWidget::SCP(QString IP)
{
QString TransferFile;
QString A=StringPop(); //Location
QString B=StringPop1(); // User
QString I=IP;//IP
string ier;
if(IP.toStdString().substr(0, 1).compare(" ")==0) {
    for(int i = 1; i<IP.length(); i++) {
        if(IP.toStdString().substr(i, i+1).compare(" ")!=0) {
            ier = IP.toStdString().substr(i+1, IP.length());
            I = QString::fromStdString(ier);
            break;
        }
    }
}
QString C=Popup(); //Desktop,Documents,Downloads
TransferFile="Scp "+A+" "+B+"@"+I+":"+"/Users/"+B+C;
cout<<TransferFile.toStdString()<<endl;
cout<<exec(TransferFile.toStdString())<<endl;
message("Transfer Complete");
}

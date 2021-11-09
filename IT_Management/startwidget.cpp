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
void StartWidget::textSetting(int tabbe) {
    if(tab->indexOf(settings_widget)==tabbe) {
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


    change_log->setFixedHeight(35);

    change_u->addWidget(change_use);
    change_u->addWidget(change_user);
    change_p->addWidget(change_pas);
    change_p->addWidget(change_pass);

    login_layout->addWidget(change_log);
    login_layout->addLayout(change_u);
    login_layout->addLayout(change_p);
    login_layout->addWidget(change_login);

    login_layout->setSpacing(3);
   // login_layout->setContentsMargins(0, 0, 20, 20);
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
    description->setFixedHeight(110);
    description->setContentsMargins(0, 8, 0, 8);
    description->setFont(font);

    description_text->setSpacing(3);
    //description_text->setContentsMargins(0, 10, 0, 50);

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
    namer_button->setText("Rename the IP");
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
    namer->addWidget(namer_button);

    namerr->addWidget(label_name);
    namerr->addWidget(IP_namers);


    IP_namer->addWidget(description_name);
    IP_namer->addLayout(namer);
    IP_namer->addLayout(namerr);

    change_full->addLayout(IP_namer);



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

    change_full->addWidget(scanner);
    change_full->addLayout(scans_hard);
    change_full->addLayout(scans_netw);
    change_full->addLayout(scans_soft);

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

void StartWidget::blackList() {
    string iper = to_IP(IP_remover_z, IP_remover_o, IP_remover_t, IP_remover_th) + "  ";

    int copy = -1;
    int i = 0;
    if(iper.compare("")!=0) {
        for(vector<string>::iterator IPss = blackList_IP.begin(); IPss!=blackList_IP.end(); IPss++, i++) {
            if(iper.compare(*IPss)==0) {
                copy = i;
            }
        }
        if(copy==-1) {
            blackList_IP.push_back(iper);
            message("IP successfully added to black List!!");
        } else {
            blackList_IP[copy].erase();
            message("IP successfully removed from black List!!");
        }

        ofstream ofile;
        ofile.open("../blackList.txt");
        for(vector<string>::iterator IPss = blackList_IP.begin(); IPss!=blackList_IP.end(); IPss++) {
            ofile<<*IPss<<endl;
        }
        ofile.close();


    }
}

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
     for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++) {

         cout<<exec(com + *lss);
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
    label_t = new QLabel(login_widget);
    QVBoxLayout *login_layout = new QVBoxLayout();
    QHBoxLayout *User_lay = new QHBoxLayout();
    QHBoxLayout *Pass_lay = new QHBoxLayout();
    QLabel *user_label = new QLabel(login_widget);
    QLabel *pass_label = new QLabel(login_widget);
    login_button = new QPushButton("Login", login_widget);
    label->setText("Login in with the details you previously used");
    label_t->setText("If you are new set a user name and password by typing credentials then click login");
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

    login_layout->addWidget(label);
    login_layout->addWidget(label_t);
    login_layout->addSpacing(10);
    login_layout->addLayout(User_lay);
    login_layout->addSpacing(10);
    login_layout->addLayout(Pass_lay);
    login_layout->addSpacing(10);
    login_layout->addWidget(login_button);
    connect(login_button, SIGNAL(clicked()), this, SLOT(loggerIn()));
    login_widget->setLayout(login_layout);

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
            label_t->setText("Invalid Credentials try agian");
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
    cout << exec(commer+com);
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
   string line;
       while(getline(ifile, line)) {
           blackList_IP.push_back(line);
       }
       ifile.close();

   //This parses the output string to only get the IPs of the output
   std::vector<string> ls;
   std::istringstream f(ts);
      while (std::getline(f, line)) {
          int current = 0;
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
                     if(l->substr(0, l->length()).compare(line.substr(2, i))==0) {
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

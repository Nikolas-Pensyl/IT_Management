#include "startwidget.h"
// include OS

//Run winbdoiwremnote.exe
using namespace std;

StartWidget::StartWidget(QWidget *parent) : QWidget(parent) {
    //Create tabs

    tab = new QTabWidget(this);
    IPlist_widget.push_back(new QTabWidget);
    settings_widget = new QTabWidget;
    login_widget = new QTabWidget;



    //Set the tabs to the Screen

    login();
    CompsbyIP();
    settings();
    validateIntervalScan();
    tab->addTab(login_widget, "login");
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(textSetting(int)));
    tab->setFixedSize(700, 500);
}


void StartWidget::textSetting(int tabbe) {
    if(tab->indexOf(settings_widget)==tabbe) {
        change_user->setText(QString::fromStdString(username));
        change_pass->setText(QString::fromStdString(password));
        scan_hard->setText(QString::fromStdString(to_string(hardware_scan)));
        scan_soft->setText(QString::fromStdString(to_string(software_scan)));
        scan_netw->setText(QString::fromStdString(to_string(network_scan)));
    }
}

void StartWidget::reScan() {
    int i = 0;
    for(vector<QTabWidget*>::iterator tabber = IPlist_widget.begin(); tabber != IPlist_widget.end(); tabber++, i++) {
        IPlist_widget[i]->clear();
        //tab->removeTab(tab->indexOf(IPlist_widget[i]));
    }
    myHLayouts.clear();
    total_layout.clear();
    IPlist_widget.clear();
    comps.clear();
    comm.clear();
    label->clear();
    label_t->clear();
    LAN.clear();
    LAN = ScanLAN();
    CompsbyIP();
    tab->repaint();
}

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
}

void StartWidget::message(QString str) {
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void StartWidget::settings() {
    //Change login
    change_log = new QLabel(settings_widget);
    change_use = new QLabel(settings_widget);
    change_pas = new QLabel(settings_widget);
    change_log->setText("Change your login info here");
    change_use->setText("Username: ");
    change_pas->setText("Password: ");
    change_user = new QLineEdit();
    change_pass = new QLineEdit();
    change_user->setText(QString::fromStdString(username));
    change_pass->setText(QString::fromStdString(password));
    change_login = new QPushButton();
    change_login->setText("Change Login");
    connect(change_login, SIGNAL(clicked()), this, SLOT(changeLogin()));
    change_u = new QHBoxLayout();
    change_p = new QHBoxLayout();

    change_full = new QVBoxLayout();

    change_u->addWidget(change_use);
    change_u->addWidget(change_user);

    change_p->addWidget(change_pas);
    change_p->addWidget(change_pass);

    change_full->addWidget(change_log);
    change_full->addLayout(change_u);
    change_full->addLayout(change_p);

    change_full->addWidget(change_login);

    remover_butt = new QPushButton();
    IP_remover = new QLineEdit();
    remover = new QLabel(settings_widget);
    rem_name = new QLabel(settings_widget);
    removel = new QHBoxLayout();
    rem_name->setText("Name/IP");
    remover->setText("Enter the name or IP Adress you want ignored on the next scan.");
    remover_butt->setText("Press to ignore entered Computer/IP");

    removel->addWidget(rem_name);
    removel->addWidget(IP_remover);
    removel->addWidget(remover_butt);

    change_full->addWidget(remover);
    change_full->addLayout(removel);



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
    scan_net->setText("Scan Network Interval");
    scan_sot->setText("Scan Software Interval");
    minutes_txt->setText("minutes");
    minutes_tx->setText("minutes");
    minutes_t->setText("minutes");

    scans_sot = new QPushButton();
    scans_had = new QPushButton();
    scans_net = new QPushButton();

    scans_had->setText("Set Hardware Scan Interval");
    scans_sot->setText("Set Sofware Scan Interval");
    scans_net->setText("Set Network Scan Interval");

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

StartWidget::~StartWidget()
{
}

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

        QPushButton  *labelbed =new QPushButton(QString::fromStdString(*lss), IPlist_widget[tabb]);
        comps.push_back(labelbed);
        connect(comps[k],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(comps[k], QString::fromStdString(*lss));


        myHLayouts[numm]->addWidget(comps[k]);
    }
    connect (signalMapper, SIGNAL(mapped(QString)), this, SLOT(execut(QString))) ;
    total_layout[tabb]->addLayout(myHLayouts[numm]);
    for(int i = 0; i!=tabb+1; i++) {
        if(i>init) {
            IPlist_widget[i]->setLayout(total_layout[i]);
            init = i;
        }
    }

}

void setCompsWid() {

}

void StartWidget::runAll(QString cmd) {
    string com = comm[tab->currentIndex()]->currentText().toStdString();
     for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++) {

         cout<<exec(com + *lss);
     }
}

void StartWidget::login() {
    string user, passw, txt;
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
}

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

void StartWidget::loggerIn() {

    bool use = false;
    bool pas = false;
    ifstream file;

    string txt, user, passw;
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
    if(pas && use) {
        if(usern->text().toStdString().compare(user)==0) {
            if(passwo->text().toStdString().compare(passw)==0) {
                logged = true;

            }
        }
        if(!logged) {
            label_t->setText("Invalid Credentials try agian");
        }
    } else {
        ofstream ofile;
        ofile.open("../logininfo.txt");
        ofile<<"username:"<<usern->text().toStdString()<<endl;
        ofile<<"password:"<<passwo->text().toStdString();
        logged = true;
    }
    if(logged) {
        username = usern->text().toStdString();
        password = passwo->text().toStdString();
        int i =1;
        for(vector<QTabWidget*>::iterator tabber = IPlist_widget.begin(); tabber != IPlist_widget.end(); tabber++, i++) {
            string nam = to_string(i);
            QString name = "Computers " + QString::fromStdString(nam);
            tab->addTab(*tabber, name);
        }
        tab->addTab(settings_widget, "Settings");
        tab->removeTab(0);
        repaint();
    }
    file.close();
}

void StartWidget::execut(QString cmd) {
    string com = cmd.toStdString();
    string commer =comm[tab->currentIndex()]->currentText().toStdString();
    cout << exec(commer+com);
}

//Send the command to the command line and return its output
//for a multi-command do && between commands for two commands
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

vector<string> StartWidget::ScanLAN() {
    //the '&&' runs multiple commands
   string ts = exec("arp -a");

   cout << ts;

   //This parses the output string to only get the IPs of the output
   std::vector<string> ls;
   std::istringstream f(ts);
      std::string line;
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
                 if(!dou) {
                     ls.push_back(line.substr(0, i));
                     cout << line.substr(0, i) <<endl;
                 } else {
                     cout<<"test"<<endl;
                 }
                 current = 0;
             }

          }

      }
    return ls;
}



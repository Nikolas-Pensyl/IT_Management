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
    //tab->addTab(IPlist_widget, "Computers");
    //tab->addTab(settings_widget, "Settings");
    tab->addTab(login_widget, "login");
    tab->setFixedSize(700, 500);
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
        IPlist_widget[i]->setLayout(total_layout[i]);
    }

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
                 ls.push_back(line.substr(0, i));
                 cout << line.substr(0, i) <<endl;
                 current = 0;
             }

          }

      }
    return ls;
}



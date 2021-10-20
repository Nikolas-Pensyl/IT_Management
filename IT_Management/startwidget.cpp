#include "startwidget.h"
// include OS

//Run winbdoiwremnote.exe
using namespace std;
using namespace std::experimental::filesystem;

StartWidget::StartWidget(QWidget *parent) : QWidget(parent) {
    //Create tabs
    QTabWidget* tab;
    tab = new QTabWidget(this);
    IPlist_widget = new QTabWidget;
    settings_widget = new QTabWidget;
    login_widget = new QTabWidget;



    //Set the tabs to the Screen
    login();
    CompsbyIP();
    tab->addTab(IPlist_widget, "Computers");
    tab->addTab(settings_widget, "Settings");
    //tab->addTab(login_widget, "login");
    tab->setFixedSize(700, 500);
}

StartWidget::~StartWidget()
{
}

void StartWidget::CompsbyIP() {
    //Scan LAN
    LAN = ScanLAN();
    //Set buttons for IPs scanned on LAN
    fullLayout = new QVBoxLayout;
    QHBoxLayout *labs = new QHBoxLayout();
    QLabel *label = new QLabel(IPlist_widget);
    label->setText("When a button is clicked, run command this command: ");
    labs->addWidget(label);
    labs->addSpacing(10);
    myHLayouts.push_back(new QHBoxLayout);
    QComboBox *comm = new QComboBox(IPlist_widget);
    comm->addItems(commands);
    labs->addWidget(comm);
    labs->addSpacing(10);
    QPushButton *butt_all = new QPushButton();
    butt_all->setText("Run command for all");
    labs->addWidget(butt_all);


    QSignalMapper* sigMapper = new QSignalMapper (this) ;
    connect(butt_all, SIGNAL(clicked()), sigMapper, SLOT(map()));
    sigMapper->setMapping(butt_all, comm->currentText());
    connect(sigMapper, SIGNAL(mapped(QString)), this, SLOT(runAll(QString)));

    fullLayout->addLayout(labs);
    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    int i = 0, num =0;
    for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++, i++) {
        if(i%5==0 && i!=0) {
            fullLayout->addLayout(myHLayouts[num]);
            num ++;
            myHLayouts.push_back(new QHBoxLayout);
        }
       QPushButton  *labelbed =new QPushButton(QString::fromStdString(*lss), IPlist_widget);
       comps.push_back(labelbed);
       connect(comps[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
       signalMapper->setMapping(comps[i], QString::fromStdString(*lss));


       myHLayouts[num]->addWidget(comps[i]);
    }
    connect (signalMapper, SIGNAL(mapped(QString)), this, SLOT(execut(QString))) ;
    fullLayout->addLayout(myHLayouts[num]);
    IPlist_widget->setLayout(fullLayout);
}

void StartWidget::runAll(QString cmd) {
    string com = cmd.toStdString();
     for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++) {
         exec(com + *lss);
     }
}

void StartWidget::login() {
    string user, passw, txt;
    QVBoxLayout *login_layout = new QVBoxLayout();
    QHBoxLayout *User_lay = new QHBoxLayout();
    QHBoxLayout *Pass_lay = new QHBoxLayout();
    QLabel *label = new QLabel(login_widget);
    QLabel *user_label = new QLabel(login_widget);
    QLabel *pass_label = new QLabel(login_widget);
    QLineEdit *usern = new QLineEdit();
    QLineEdit *passwo = new QLineEdit();
    QPushButton *login_button = new QPushButton("Login", login_widget);
    label->setText("Login in with the details you previously used");

    user_label->setText("Username:");
    pass_label->setText("Password:");

    User_lay->addWidget(user_label);
    User_lay->addSpacing(10);
    User_lay->addWidget(usern);

    Pass_lay->addWidget(pass_label);
    Pass_lay->addSpacing(10);
    Pass_lay->addWidget(passwo);

    login_layout->addWidget(label);
    login_layout->addSpacing(10);
    login_layout->addLayout(User_lay);
    login_layout->addSpacing(10);
    login_layout->addLayout(Pass_lay);
    login_layout->addSpacing(10);
    login_layout->addWidget(login_button);
    login_widget->setLayout(login_layout);
}
   /* fstream file;
    file.open("logininfo.txt");
    if(std::experimental::filesystem::exists("logininfo.txt")) {
        label->setText("Login in with the details you previously used");
        ifstream infile;
        infile.open("logininfo.txt");
        while(getline(infile, txt)) {
            if(txt.find("username:")!=string::npos) {
                user = txt.substr(9, txt.length());
            } else if(txt.find("password:")!=string::npos) {
                passw = txt.substr(9, txt.length());
            }
        }
        if(usern->text().toStdString().compare(user)==0) {
            if(passwo->text().toStdString().compare(passw)==0) {
                logged = true;
            }
        }
        infile.close();
    } else {

        label->setText("You are setting your username and password for the next time you sign in.");
        ofstream ofile;
        ofile.open("logininfo.txt");
        ofile<<"username:"<<usern->text().toStdString();
        ofile<<"password:"<<passwo->text().toStdString();
        ofile.close();
        logged = true;
    }
    login_layout->addWidget(label);
    login_layout->addWidget(usern);
    login_layout->addWidget(passwo);
}*/

void StartWidget::execut(QString cmd) {
    string com = cmd.toStdString();
    cout << exec("ping "+com);
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



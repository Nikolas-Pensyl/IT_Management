#include "startwidget.h"
// include OS

//Run winbdoiwremnote.exe
using namespace std;
StartWidget::StartWidget(QWidget *parent) : QWidget(parent) {
    QTabWidget* tab;
    tab = new QTabWidget(this);
    IPlist_widget = new QTabWidget;
    IPlist_widget->setStyleSheet("background-color:black;");
    settings_widget = new QTabWidget;
    settings_widget->setStyleSheet("background-color:red;");
    login_widget = new QTabWidget;
    LAN = ScanLAN();
    myHLayouts.push_back(new QHBoxLayout);
    fullLayout = new QVBoxLayout;
    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    int i = 0, num =0;
    for(std::vector<string>::iterator lss = LAN.begin(); lss!=LAN.end(); lss++, i++) {
        if(i%5==0 && i!=0) {
            fullLayout->addLayout(myHLayouts[num]);
            num ++;
            myHLayouts.push_back(new QHBoxLayout);
        }
       QPushButton  *labelbed =new QPushButton(QString::fromStdString(*lss), login_widget);
       comps.push_back(labelbed);
       connect(comps[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
       signalMapper->setMapping(comps[i], QString::fromStdString(*lss));


       myHLayouts[num]->addWidget(comps[i]);
    }
    connect (signalMapper, SIGNAL(mapped(QString)), this, SLOT(execut(QString))) ;
    fullLayout->addLayout(myHLayouts[num]);
    login_widget->setLayout(fullLayout);
    IPlist_widget->setStyleSheet("background-color:blue;");
    tab->addTab(IPlist_widget, "Computers by IP");
    tab->addTab(settings_widget, "Settings");
    tab->addTab(login_widget, "login");
    tab->setFixedSize(700, 500);
}

StartWidget::~StartWidget()
{
}

void StartWidget::execut(QString cmd) {
    string com = cmd.toStdString();
    cout << exec("ping "+com);
}

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
    return result;
 }

vector<string> StartWidget::ScanLAN() {
    //the '&&' runs multiple commands
   string ts = exec("arp -a");

   cout << ts;
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



#include "widget.h"
#include "QString"



Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
   //Popup();
   //StringPop();
   //StringPop1();
    SCP();


}

string exec(string command) {
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

Widget::~Widget()
{
}

QString Widget::Popup() {
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

QString Widget::StringPop() {
    bool ok;
    QString location;
    QString blank = "";
    QString text = QInputDialog::getText(this, tr("Location"), tr("Please enter the file location of what file you wish to transfer:"), QLineEdit::Normal, blank, &ok);

    location=text;
    location.toStdString();
return location;
}

QString Widget::StringPop1() {
    bool ok;
    QString TargetUser;
    QString blank1 ="";
    QString text1 = QInputDialog::getText(this, tr("Target User"), tr("Please enter the target User you which to transfer to:"), QLineEdit::Normal, blank1, &ok);

    TargetUser=text1;
    TargetUser.toStdString();
return TargetUser;
}

QString Widget::StringPop2() {
    bool ok;
    QString IP;
    QString blank1 ="";
    QString text1 = QInputDialog::getText(this, tr("IP"), tr("Please enter the IP address you which to transfer to:"), QLineEdit::Normal, blank1, &ok);

    IP=text1;
    IP.toStdString();
return IP;
}

void Widget::SCP()
{
QString TransferFile;
QString A=StringPop(); //Location
QString B=StringPop1(); // User
QString I=StringPop2();//IP
QString C=Popup(); //Desktop,Documents,Downloads
TransferFile="Scp "+A+" "+B+"@"+I+":"+"/Users/"+B+C;
cout<<TransferFile.toStdString()<<endl;
cout<<exec(TransferFile.toStdString())<<endl;
}


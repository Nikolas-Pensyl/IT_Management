#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <string>
#include <QMessageBox>
#include <QInputDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <iostream>

using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QString Popup();
    QString StringPop();
    QString StringPop1();
    QString StringPop2();
    void SCP();
};
#endif // WIDGET_H

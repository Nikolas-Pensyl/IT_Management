#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QTabWidget>

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();
private:
    QTabWidget* IPlist_widget;
    QTabWidget* login_widget;
    QTabWidget* settings_widget;
};
#endif // STARTWIDGET_H

#ifndef WIDGET_H
#define WIDGET_H
#include"mainwindow.h"
#include"form.h"
#include <QWidget>
#include<QDebug>
#include<QtNetwork>
#include<QMessageBox>
#include <QTextCodec>
#include <QTcpSocket>
#include <QtGui>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void with();
    void ret();
    void get();
private:
    Ui::Widget *ui;
   QTcpSocket sock;
};

#endif // WIDGET_H

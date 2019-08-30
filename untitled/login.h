#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <mainwindow.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_clicked();
    void do_something();
    void handConnect();
    void handData();

private:
    Ui::Login *ui;
    QTcpSocket sock;
};

#endif // LOGIN_H

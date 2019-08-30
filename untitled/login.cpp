#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(do_something()));

    sock.connectToHost("127.0.0.1", 1234);
    connect(&sock, SIGNAL(connected()), this, SLOT(handConnect()));
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    MainWindow * mWind = new MainWindow(&sock);
    mWind->show();
    this->hide();






    QString uName = ui->lineEdit->text();
    QString pWord = ui->lineEdit_2->text();
    if (uName.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入用户名");
    }else if (pWord.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入密码");
    } else {
        QString sendBuf = "2|" + uName + "|" + pWord;
        qDebug() << sendBuf;

        sock.write(sendBuf.toUtf8());

    }
}

void Login::do_something()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Login::handConnect() {
    ui->pushButton->setEnabled(true);
    connect(&sock, SIGNAL(readyRead()), this, SLOT(handData));
}

void Login::handData() {
    QByteArray recvBuf = sock.readAll();
    QString buf(recvBuf);
    qDebug() << buf;
    //0|xxxxx -> success!
    QStringList slist = buf.split("|");
    if (slist.at(0) == "0") {
        QMessageBox::information(this, "提示", "登陆成功！");
        MainWindow * mWind = new MainWindow();
        mWind->show();
        this->hide();
    } else {
        QMessageBox::warning(this, "提示", slist.at(1));
    }
}

#include "form.h"
#include "gv.h"
#include "ui_form.h"

Form::Form(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    connect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    QString uName = ui->lineEdit_2->text();
    QString pWord = ui->lineEdit_3->text();
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入正确的用户名");
    }
    if (ui->lineEdit_2->text() != ui->lineEdit_3->text()) {
        QMessageBox::warning(this, "提示", "两次输入密码不相同");
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        return;
    }
    QString sendBuf = "1|"; //协议改写处
    sendBuf += uName;
    sendBuf += "|";
    pWord = QCryptographicHash::hash(pWord.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
    sendBuf += pWord;
    qDebug() << sendBuf;
    sock.write(sendBuf.toUtf8());
}
void Form::ret()
{
    QByteArray recvBuf = sock.readAll();
    QString buf(recvBuf);
    qDebug() << buf;
    QStringList slist = buf.split("|"); //分割
    if (slist.at(0) == "0") {
        QMessageBox::information(this, "恭喜", "注册成功.");
        disconnect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
        Widget* w = new Widget();
        w->show();
        this->close(); //hide?
    } else {
        QMessageBox::information(this, "很遗憾", "注册失败,请修改用户名");
    }
}

#include "form.h"
#include "ui_form.h"

Form::Form(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{

    ui->setupUi(this);
    connect(&confd, SIGNAL(readyRead()), this, SLOT(ret()));
}
/*Form::Form(QTcpSocket *sock,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    confd=sock;
    ui->setupUi(this);

}
*/

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    confd.connectToHost("10.194.55.155", 1234);
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
    sendBuf += pWord;
    qDebug() << sendBuf;
    confd.write(sendBuf.toUtf8());
}
void Form::ret()
{
    QByteArray recvBuf = confd.readAll();
    QString buf(recvBuf);
    qDebug() << buf;
    QStringList slist = buf.split("|"); //分割
    if (slist.at(0) == "0") {
        QMessageBox::information(this, "恭喜", "注册成功.");
        confd.close();
    } else {
        QMessageBox::information(this, "很遗憾", "注册失败,请修改用户名");
    }
}

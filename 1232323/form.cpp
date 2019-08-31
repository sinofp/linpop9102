#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    QString uName= ui->lineEdit_2->text();
       QString pWord=ui->lineEdit_3->text();
    if(ui->lineEdit_2->text()!=ui->lineEdit_3->text())
        {
        QMessageBox::warning(this,"提示","两次输入密码不相同");
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        return;
    }
    else {
        QString sendBuf="1|";//协议改写处
        sendBuf+=uName;
        sendBuf+="|";
        sendBuf+=pWord;
        qDebug()<<sendBuf;
         //  sock.write(sendBuf.toUtf8());//把连接弄过来，新讲的
         QMessageBox::information(this,"恭喜","注册成功");
    }
}

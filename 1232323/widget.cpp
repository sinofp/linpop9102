#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    sock.connectToHost("192.168.29.1",1234);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(get()));
    connect(&sock,SIGNAL(connected()),this,SLOT(with));
connect(&sock,SIGNAL(readyRead()),this,SLOT(ret()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()//登录槽
{
    QString uName= ui->lineEdit->text();
       QString pWord=ui->lineEdit_2->text();
       if(uName.isEmpty())
           //
           {
           QMessageBox::warning(this,"tishi","用户名不能为空");
           return;
                   }
       if(pWord.isEmpty())
       {

           QMessageBox::warning(this,"tishi","密码不能为空");
           return;
       }
       QString sendBuf="2|";//协议改写处
       sendBuf+=uName;
       sendBuf+="|";
       sendBuf+=pWord;
       qDebug()<<sendBuf;
        sock.write(sendBuf.toUtf8());
}
void  Widget::with()//客户端连接
{
     ui->pushButton->setEnabled(true);
   // connect(&sock,SIGNAL(readyRead()),this,SLOT(ret()));
}
void Widget::on_pushButton_2_clicked()//创建槽
{
    Form *zhuce=new Form();
    zhuce->show();
}
void Widget::get()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Widget::ret()//返回信息
{   qDebug()<< "xcsc";
    QByteArray recvBuf=sock.readAll();
    QString buf(recvBuf);
    qDebug()<<buf;
    QStringList slist= buf.split("|");//分割
    if(slist.at(0)=="0")
    {
        QMessageBox::information(this,"zheng","登录成功");
        MainWindow *mWind=new MainWindow();
        mWind->show();
       // this->hide();
        return;
    }else //这里当与正式服务端相连时变更
    {
        QMessageBox::warning(this,"错误",slist.at(1));
    }
}



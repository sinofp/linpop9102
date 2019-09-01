#include "widget.h"
#include "ui_widget.h"
#include "gv.h"
Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(get()));
    connect(&sock, SIGNAL(connected()), this, SLOT(with()));
    connect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
}

//void Widget::connect_socket() {
//    connect(&sock, SIGNAL(connected()), ui->wi, SLOT(with()));
//    connect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
//}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked() //登录槽
{
//    if (QAbstractSocket::SocketError(close()))
//        sock.connectToHost("127.0.0.1", 1234);
    QString uName = ui->lineEdit->text();
    QString pWord = ui->lineEdit_2->text();
    if (uName.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名不能为空");
    } else if (pWord.isEmpty()) {
        QMessageBox::warning(this, "提示", "密码不能为空");
    }
    QString sendBuf = "2|"; //协议改写处
    sendBuf += uName;
    sendBuf += "|";
    pWord = QCryptographicHash::hash(pWord.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
    sendBuf += pWord;
    qDebug() << sendBuf;
    sock.write(sendBuf.toUtf8());
}
void Widget::with() //客户端连接
{
    ui->pushButton->setEnabled(true);
}
void Widget::on_pushButton_2_clicked() //创建槽
{

    Form* zhuce = new Form();
    zhuce->show();
    // 把connect过的socket方法全都解绑
    disconnect(&sock, SIGNAL(connected()), this, SLOT(with()));
    disconnect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
    this->close();//hide? 问题是回到它怎么绑定socket，干脆再执行一遍构造函数好了
}
void Widget::get()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Widget::ret() //返回信息
{
    QByteArray recvBuf = sock.readAll();
    QString buf(recvBuf);
    qDebug() << buf;
    QStringList slist = buf.split("|"); //分割
    if (slist.at(0) == "0") {
        QMessageBox::information(this, "zheng", "登录成功");
        MainWindow* mWind = new MainWindow();
        mWind->show();
        this->close(); //hide?
    } else //这里当与正式服务端相连时变更
    {
        QMessageBox::warning(this, "错误", slist.at(1));
    }
}

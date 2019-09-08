#include "widget.h"
#include "gv.h"
#include "ui_widget.h"
Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(get()));
    connect(&sock, SIGNAL(connected()), this, SLOT(with()));
    connect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->lineEdit_2->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked() //登录槽
{
    uName = ui->lineEdit->text();
    QString pWord = ui->lineEdit_2->text();
    if (uName.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名不能为空");
    } else if (pWord.isEmpty()) {
        QMessageBox::warning(this, "提示", "密码不能为空");
    }
    Message a;
    a.msgType = LOGIN;
    QByteArray str = uName.toUtf8();
    strcpy(a.sendName, str.data());
    QString b;
    pWord = QCryptographicHash::hash(pWord.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
    b += pWord;
    str = b.toUtf8();
    strcpy(a.content, str.data());
    qDebug() << a.sendName;
    sock.write((char*)&a, sizeof(a));
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
    this->close(); //hide? 问题是回到它怎么绑定socket，干脆再执行一遍构造函数好了
}
void Widget::get()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Widget::ret() //返回信息
{
    //    QByteArray ba = sock.readAll();
    //    qDebug() << ba.length();
    Message Buf;
    qint64 recv_len = sock.read((char*)&Buf, sizeof(Buf));
    qDebug() << recv_len;
    if (Buf.msgRet == SUCCESS) {
        //        QMessageBox::information(this, "zheng", "登录成功");
        MainWindow* mWind = new MainWindow(uName);
        mWind->show();
        disconnect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
        this->close(); //hide?
    } else if (Buf.msgRet == ID_NOT_EXIST) //这里当与正式服务端相连时变更
    {
        QMessageBox::warning(this, "错误", "账号不存在");
    } else if (Buf.msgRet == WRONGPWD) {
        QMessageBox::warning(this, "错误", "密码错误");
    }
}

void Widget::on_lineEdit_editingFinished()
{
    QString user_name = ui->lineEdit->text();
    if (user_name.isEmpty()) {
        ui->avatar_label->hide();
    } else {
        ui->avatar_label->setPixmap(QPixmap::fromImage(gen_idcon(user_name).scaled(ui->avatar_label->size())));
        ui->avatar_label->show();
    }
}
void Widget::paintEvent(QPaintEvent*)
{
    QLinearGradient linear(rect().topLeft(), rect().bottomRight());
    linear.setColorAt(0, QColor(215, 231, 231));
    linear.setColorAt(0.5, QColor(220, 220, 220));
    linear.setColorAt(1, QColor(215, 231, 231));

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::transparent);
    //p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 20, 20);
    setMask(bmp);
    QPainter painter(this);
    painter.setBrush(QBrush(linear));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
}

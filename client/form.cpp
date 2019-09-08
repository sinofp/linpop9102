#include "form.h"
#include "gv.h"
#include "ui_form.h"

Form::Form(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

    connect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));

    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->lineEdit_2->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->lineEdit_3->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->pushButton->setStyleSheet("QPushButton{background-color:rgba(255,178,0,100%);"
                                  "\ color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}"
                                  "QPushButton:hover{background-color:white; color: black;}" // 鼠标停放时的色彩
                                  "QPushButton:pressed{background-color:rgb(176,196,222); border-style: inset; }"); // 鼠标按下的色彩
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    QString uName = ui->lineEdit->text();
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
    Message a; //消息转换
    a.msgType = REGISTER;
    QByteArray str = uName.toUtf8();
    strcpy(a.sendName, str.data());
    QString sendBuf;
    pWord = QCryptographicHash::hash(pWord.toLocal8Bit(), QCryptographicHash::Sha256).toHex();
    sendBuf += pWord;
    str = sendBuf.toUtf8(); //content mima
    strcpy(a.content, str.data());
    qDebug() << a.content;
    qDebug() << a.sendName;
    sock.write((char*)&a, sizeof(a));
}
void Form::ret()
{
    Message Buf;
    sock.read((char*)&Buf, sizeof(Buf));
    if (Buf.msgRet == SUCCESS) {
        QMessageBox::information(this, "恭喜", "注册成功.");
        disconnect(&sock, SIGNAL(readyRead()), this, SLOT(ret()));
        Widget* w = new Widget();
        w->show();
        this->close(); //hide?
    } else {
        QMessageBox::information(this, "很遗憾", "注册失败,请修改用户名");
    }
}
void Form::paintEvent(QPaintEvent*)
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

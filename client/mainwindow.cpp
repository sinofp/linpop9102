#include "mainwindow.h"
#include "emojiwidget.h"
#include "gv.h"
#include "progressdialog.h"
#include "string.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::MainWindow(QString my_id, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    er = my_id;
    to_user_id = my_id; //先给送达人初始化
    ui->label_2->setText(er);
    ui->tableWidget_3->verticalHeader()->setVisible(false); //隐藏行表头
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setShowGrid(false);
    ui->tableWidget_2->setColumnWidth(0, 1000); //尽可能多设置窗口宽度
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->horizontalHeader()->setVisible(false);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(&sock, SIGNAL(readyRead()), this, SLOT(retr()));
    connect(ui->tableWidget_3, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(getItem(QTableWidgetItem*)));

    //    ui->pushButton_2->setIcon(QIcon(er + ".png"));
    ui->label_5->setPixmap(QPixmap::fromImage(gen_idcon(er).scaled(ui->label_5->size())));
    //    dig=new progressdialog();

    //    connect(this,SIGNAL(emit_progress(long,long)),dig,SLOT(init_dig(long,long)));
    //    Message message;
    //    // discard one message
    //    sock.read((char*)&message, sizeof(message));

    //    int row_count = ui->tableWidget_3->rowCount();
    //    ui->tableWidget_3->insertRow(row_count);
    //    ui->tableWidget_3->setItem(row_count, 0, new QTableWidgetItem(QIcon(),""));
    //    ui->tableWidget_3->setItem(row_count, 2, new QTableWidgetItem(QString("sha")));
    //    ui->tableWidget_3->setItem(row_count, 1, new QTableWidgetItem(QString("在线")));

    //    row_count = ui->tableWidget_3->rowCount();
    //        ui->tableWidget_3->insertRow(row_count);
    //        ui->tableWidget_3->setItem(row_count, 0, new QTableWidgetItem(QIcon(),""));
    //        ui->tableWidget_3->setItem(row_count, 2, new QTableWidgetItem(QString("sha2")));
    //        ui->tableWidget_3->setItem(row_count, 1, new QTableWidgetItem(QString("在线")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_2_clicked() //发送消息
{
    memset(&msg, 0, sizeof(msg));
    msg.msgType = PERSONAL_CHAT;
    shijian = new QDateTime;
    QString nowTime = shijian->currentDateTime().toString();
    QByteArray sh = er.toUtf8();

    strcpy(msg.sendName, sh.data());
    qDebug() << "zheshiliaotian";
    qDebug() << msg.sendName;
    QString a;
    a = ui->textEdit->toPlainText(); //获得信息
    if (a == "")
        return;
    sh = to_user_id.toUtf8(); //获取用户消息
    strcpy(msg.recvName, sh.data());
    sh = a.toUtf8();
    strcpy(msg.content, sh.data());
    ui->textEdit->clear();
    int row_count = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(row_count);
    ui->tableWidget_2->insertRow(row_count + 1);
    ui->tableWidget_2->setItem(row_count, 0, new QTableWidgetItem("我:" + QString(nowTime)));
    ui->tableWidget_2->setItem(row_count + 1, 0, new QTableWidgetItem(QString(msg.content)));
    qDebug() << msg.content;
    qDebug() << "sock.write";
    sock.write((char*)&msg, sizeof(msg));
}
void MainWindow::retr() //收信息
{
    qDebug() << "MainWindow::retr()";
    Message Buf;
    qint64 recv_len = sock.read((char*)&Buf, sizeof(Buf));
read_again:
    qDebug() << recv_len;
    qDebug() << Buf.msgType;

    //    if(Buf.msgType==FL)
    //    {
    //        memset(&fileName,0,sizeof(0));
    //        fileName=QString(Buf.content).section("&",0,0);
    //        fileSize=QString(Buf.content).section("&",1,1).toInt();
    //        qDebug()<<"FL:"<<fileName<<fileSize;
    //        recvSize=0;
    //        filesend.setFileName(fileName);
    //        if(!filesend.open(QIODevice::WriteOnly))
    //        {
    //            qDebug()<<"此文件无法接收";
    //            return;
    //        }
    //        else
    //        {
    //            qDebug()<<"此文件kaishi接收";
    //        }

    //    }
    if (Buf.msgType == FL_CONTENT) {
        QString str = Buf.content;

        shijian = new QDateTime;
        QString nowTime = "";
        nowTime.append(er);
        nowTime.append(":");
        nowTime.append(shijian->currentDateTime().toString());

        int row_count = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row_count);
        ui->tableWidget_2->insertRow(row_count + 1);
        ui->tableWidget_2->setItem(row_count, 0, new QTableWidgetItem(nowTime));
        // row_count = ui->tableWidget_2->rowCount();
        // ui->tableWidget_2->insertRow(row_count);
        ui->tableWidget_2->setItem(row_count + 1, 0, new QTableWidgetItem(QIcon(str), ""));
    }
    if (Buf.msgType == PERSONAL_CHAT) //收消息加显示消息
    {
        if (Buf.sendName != to_user_id) {
            qDebug() << to_user_id;
            int row = ui->tableWidget_3->rowCount();
            //    int g;
            //  int hei=0;
            /* for(g=0;g<row;g++)
           {
               hei=0;
               QString s;
               s=ui->tableWidget_3->item(g,1)->text();
               if(Buf.sendName==s)
               {
                   hei=1;
                   break;
               }

           }
           if(hei!=1)
           {   */
            QString cmd = Buf.sendName;
            cmd.append("给你了一个消息");
            ui->label_4->setText(cmd);
        }
        //    to_user_id = Buf.sendName;
        QString str = Buf.content;

        shijian = new QDateTime;
        QString nowTime = "";
        nowTime.append(Buf.sendName);
        nowTime.append(":");
        nowTime.append(shijian->currentDateTime().toString());

        int row_count = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row_count);
        ui->tableWidget_2->insertRow(row_count + 1);
        ui->tableWidget_2->setItem(row_count, 0, new QTableWidgetItem(QString(nowTime)));
        ui->tableWidget_2->setItem(row_count + 1, 0, new QTableWidgetItem(QString(Buf.content)));
        // QTableWidgetItem* item = ui->tableWidget_2->item(row_count,0);
        // item->setTextAlignment(Qt::AlignLeft);
    }
    if (Buf.msgType == ADD_FRIEND) { //加好友显

        int row_count = ui->tableWidget_3->rowCount();
        int flag = 0;
        int i;
        QString a;
        if (Buf.sendName == er)
            a = Buf.recvName;
        else {
            a = Buf.sendName;
        }
        ui->tableWidget_3->insertRow(row_count);
        gen_idcon(a);
        ui->tableWidget_3->setItem(row_count, 0, new QTableWidgetItem(QIcon(QString(a) + ".png"), ""));
        ui->tableWidget_3->setItem(row_count, 1, new QTableWidgetItem(QString(a)));
    }
    if (Buf.msgType == VIEW_RECORDS) //登录后显示好友列表
    {
        qDebug() << "uf.msgType==VIEW_RECORDS";
        QString b = Buf.content;
        QStringList slist = b.split("|");

        int i = 0;
        for (i = 0; i < slist.length() - 1; i++) {
            qDebug() << slist.at(i);
            QStringList fenge = slist.at(i).split(",");
            qDebug() << fenge.at(1);
            qDebug() << fenge.at(0); //里面在分割，看是否在线
            if (fenge.at(1) == "0") //0是我的好友
            {
                qDebug() << "shi";
                int row_count = ui->tableWidget->rowCount();
                gen_idcon(fenge.at(0));
                ui->tableWidget_3->insertRow(row_count);
                ui->tableWidget_3->setItem(row_count, 0, new QTableWidgetItem(QIcon(fenge.at(0) + ".png"), ""));
                ui->tableWidget_3->setItem(row_count, 1, new QTableWidgetItem(QString(fenge.at(0))));

            } else {
                qDebug() << "bushi";
                int row_count = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row_count);
                ui->tableWidget->setItem(row_count, 0, new QTableWidgetItem(QIcon(), ""));
                ui->tableWidget->setItem(row_count, 1, new QTableWidgetItem(QString(fenge.at(0))));
            }
        }
    }
    if (Buf.msgType == VIEW_USER_LIST) {
        qDebug() << "Buf.msgType==VIEW_USER_LIST";
        QString b = Buf.content;
        QStringList slist = b.split("|");

        int row_count = ui->tableWidget_3->rowCount();
        int j = 0;
        for (j = 0; j < row_count; j++)
            ui->tableWidget_3->setItem(j, 2, new QTableWidgetItem(QString("")));
        int i = 0;
        for (i = 0; i < slist.length() - 1; i++) {
            int flag = 0;
            for (j = 0; j < row_count; j++) {
                QString s = ui->tableWidget_3->item(j, 1)->text();
                if (s == slist.at(i)) {
                    ui->tableWidget_3->setItem(j, 2, new QTableWidgetItem(QString("在线")));
                    flag = 1;
                }
                if (flag == 1)
                    break;
            }
        }
    }
    if (Buf.msgType == DELETE_FRIEND) {

        QString b;
        if (Buf.sendName == er) {
            b = Buf.recvName;
        } else {
            b = Buf.sendName;
        }
        int row_count = ui->tableWidget_3->rowCount();
        int j = 0;
        int flag = 0;
        for (j = 0; j < row_count; j++) {
            QString s = ui->tableWidget_3->item(j, 1)->text();
            if (s == b) {
                ui->tableWidget_3->removeRow(j);
                flag = 1;
            }
            if (flag == 1)
                break;
        }
    }

    recv_len = sock.read((char*)&Buf, sizeof(Buf));
    if (recv_len > 0)
        goto read_again;
}

void MainWindow::on_toolButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);
    if (path == NULL) {
        QMessageBox::warning(this, "提示", "路径有误！");
        return;
    }

    fileName.clear();
    fileSize = 0;
    //获取文件信息
    QFileInfo info(path);
    fileName = info.fileName();
    fileSize = info.size();
    //打开文件
    filesend.setFileName(path);
    if (!filesend.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "提示", "文件打不开！");
        return;
    } else {
        QMessageBox::information(this, "提示", "成功打开文件");
        //ready_send();
        //        connect(&timer,&QTimer::timeout,[=]()
        //        {
        //           timer.stop();
        //           sendData();
        //        });
    }
}

void MainWindow::on_pushButton_clicked() //加好友
{
    //chushihua
    memset(&msg, 0, sizeof(msg));
    msg.msgType = ADD_FRIEND;
    QByteArray sh = er.toUtf8();
    strcpy(msg.sendName, sh.data());
    QString name;
    name = ui->lineEdit->text();
    ui->lineEdit->clear();
    sh = name.toUtf8();
    strcpy(msg.recvName, sh.data());
    qDebug() << msg.recvName;
    qDebug() << "jiahaoyou";
    sock.write((char*)&msg, sizeof(msg));
}

void MainWindow::on_pushButton_4_clicked() //删除好友
{
    memset(&msg, 0, sizeof(msg));
    msg.msgType = DELETE_FRIEND;
    QByteArray sh = er.toUtf8();
    strcpy(msg.sendName, sh.data());
    QString name;
    name = ui->lineEdit->text();
    ui->lineEdit->clear();
    sh = name.toUtf8();
    strcpy(msg.recvName, sh.data());
    qDebug() << msg.recvName;
    sock.write((char*)&msg, sizeof(msg));
}

void MainWindow::on_pushButton_3_clicked() //发送刷新要求
{
    memset(&msg, 0, sizeof(msg));
    msg.msgType = VIEW_USER_LIST;
    QByteArray sh = er.toUtf8();
    strcpy(msg.sendName, sh.data());
    sock.write((char*)&msg, sizeof(msg));
}
void MainWindow::getItem(QTableWidgetItem* item)
{

    to_user_id = item->text();
    ui->label_4->setText("You are talking to: " + to_user_id);
    strcpy(msg.recvName, to_user_id.toStdString().c_str());
}

//更改字体
void MainWindow::on_fontComboBox_currentFontChanged(const QFont& f)
{
    ui->tableWidget_2->setFont(f);
}

bool MainWindow::saveFile(const QString& fileName1)
{
    QFile file(fileName1);

    if (!file.open(QFile::WriteOnly | QFile ::Text)) {
        QMessageBox::warning(this, "保存文件", "无法保存文件");
        return false;
    }

    QTextStream out(&file);
    int row_count = ui->tableWidget_2->rowCount();
    int i;
    for (i = 0; i < row_count; i++) {
        out << ui->tableWidget_2->item(i, 0)->text();
        out << "\n";
    }
    return true;
}

void MainWindow::on_pushButton_5_clicked()
{

    Form12* f = NULL;
    f = new Form12();
    f->init(fileName1);

    f->show();
}

void MainWindow::on_toolButton_3_clicked()
{
    int l = ui->tableWidget_2->rowCount();
    if (l == 0)
        QMessageBox::warning(this, "警告", "聊天记录不能为空!", QMessageBox::Ok);
    else {
        //保存聊天记录
        fileName1 = QFileDialog::getSaveFileName(this, tr("保存聊天记录"), tr("聊天记录"), tr("文本(*.txt);;All File(*.*)"));
        if (!fileName1.isEmpty())

            saveFile(fileName1);

        qDebug() << fileName1;
    }
}

//void MainWindow::sendData()
//{
//    qDebug()<<"执行senddata";
//    long len=0;
//    do{
//        len=0;
//        memset(&msg,0,sizeof(msg));
//        strcpy(msg.sendName,"test");
//        strcpy(msg.recvName,"test");
//        char buf[1024]={0};
//        len=filesend.read(buf,sizeof(buf));
//        msg.msgType=FL_CONTENT;
//        msg.fileLength=len;
//        strcpy(msg.content,buf);
//        len=sock.write((char*)&msg,sizeof(msg));
//        sendSize+=len;
//       // emit emit_progress(sendSize,fileSize);

//    }while(len>0);
//    if(sendSize==fileSize)
//    {
//        QMessageBox::information(this,"提示","文件发送完毕");
//        filesend.close();
//    }
//    else {
//        QMessageBox::information(this,"提示","文件发送失败");
//        filesend.close();
//    }
//}
//void MainWindow::ready_send()
//{
//    Message msgg;
//    qDebug()<<"已经执行ready send";
//    memset(&msgg,0,sizeof(msgg));
//    strcpy(msgg.sendName,"xixi");
//    strcpy(msgg.recvName,"test");
//    msgg.msgType=FL;
//    QString head=QString("0&%1&%2").arg(fileName).arg(fileSize);
//    qDebug()<<fileName<<",,,"<<fileSize;
//    strcpy(msg.content,head.toUtf8());
//    long len=sock.write((char*)&msgg,sizeof(msgg));
//    if(len<0)
//    {
//        QMessageBox::warning(this,"提示","文件发送失败");
//        filesend.close();
//        return;
//    }
//    else {
//         QMessageBox::warning(this,"提示","文件开始发送");
//         sendData();
//    }

////    timer.start(20);//防止丢包

//    //dig->show();

//}

void MainWindow::on_toolButton_5_clicked()
{
    emojiwidget* emo = NULL;
    if (flag == 0) {
        emo = new emojiwidget();
        emo->parentt = (MainWindow*)emo->parentt;
        emo->parentt = this;
        emo->init_emotion();
        emo->move(600, 300);
        emo->show();
        flag = 1;
        connect(emo, SIGNAL(emit_emoji_path(QString)), this, SLOT(on_recv_emoji_path(QString)));

    } else {
        emo->close();
        flag = 0;
    }
}
void MainWindow::on_recv_emoji_path(QString path)
{
    qDebug() << "on_recv_emoji recv path:" << path;
    shijian = new QDateTime;
    QString nowTime = "";
    nowTime.append(shijian->currentDateTime().toString());
    int row_count = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(row_count);
    ui->tableWidget_2->insertRow(row_count + 1);
    ui->tableWidget_2->setItem(row_count, 0, new QTableWidgetItem(nowTime));
    // row_count = ui->tableWidget_2->rowCount();
    // ui->tableWidget_2->insertRow(row_count);
    ui->tableWidget_2->setItem(row_count + 1, 0, new QTableWidgetItem(QIcon(path), ""));
    //    QTableWidgetItem* item = ui->tableWidget_2->item(row_count,0);
    //     item->setTextAlignment(Qt::AlignLeft);

    Message msg;
    msg.msgType = FL_CONTENT;
    QByteArray sh = path.toUtf8();
    strcpy(msg.content, sh.data());
    sh = er.toUtf8();
    strcpy(msg.sendName, sh.data());
    sh = to_user_id.toUtf8();
    strcpy(msg.recvName, sh.data());
    sock.write((char*)&msg, sizeof(msg));

    flag = 0;
}

void MainWindow::on_pushButton_6_clicked()
{
    // sendData();
}

#include "gv.h"
#include "lhk.h"
#include "mainwindow.h"
#include "widget.h"
#include <QApplication>
#include <QTcpSocket>
QTcpSocket sock;
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    sock.connectToHost("172.20.10.12", 1234);
    //todo socket错误处理
    Widget w;
    w.show();

    return a.exec();
}

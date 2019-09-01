#include "gv.h"
#include "widget.h"
#include <QApplication>
#include <QTcpSocket>

QTcpSocket sock;
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    sock.connectToHost("127.0.0.1", 1234);
    //todo socket错误处理
//    Widget w;
    MainWindow w;
    w.show();

    return a.exec();
}

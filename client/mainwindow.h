#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "form12.h"
#include "lhk.h"
#include "progressdialog.h"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTcpSocket>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTime>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    MainWindow(QString my_id, QWidget* parent = nullptr);
    ~MainWindow();
    QString fileName1;
    int flag = 0;
    //void sendData();
    QWidget parentt;
signals:

    void emit_progress(long, long);

private slots:
    void on_pushButton_2_clicked();
    void retr();
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void on_recv_emoji_path(QString path);

    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    //  void ready_send();

    void on_pushButton_3_clicked();
    void getItem(QTableWidgetItem* item);

    void on_fontComboBox_currentFontChanged(const QFont& f);

    bool saveFile(const QString& fileName);

    void on_toolButton_3_clicked();

    void on_toolButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow* ui;
    Message msg;
    QString to_user_id;
    QString er;
    int chatType;
    QDateTime* shijian;
    QString fileName;
    // progressdialog *dig;

    QFile filesend;
    long fileSize;
    long sendSize;
    long recvSize;
    QTimer timer;
};

#endif // MAINWINDOW_H

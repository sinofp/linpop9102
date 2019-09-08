#ifndef WIDGET_H
#define WIDGET_H
#include "form.h"
#include "frilist.h"
#include "lhk.h"
#include "mainwindow.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPixmap>
#include <QTcpSocket>
#include <QTextCodec>
#include <QWidget>
#include <QtGui>
#include <QtNetwork>
#include <cstring>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent*);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_editingFinished();
    void with();
    void ret();
    void get();

private:
    Ui::Widget* ui;
    QString uName;
};

#endif // WIDGET_H

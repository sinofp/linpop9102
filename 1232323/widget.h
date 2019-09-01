#ifndef WIDGET_H
#define WIDGET_H
#include "form.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTextCodec>
#include <QWidget>
#include <QtGui>
#include <QtNetwork>
namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void with();
    void ret();
    void get();

    void on_lineEdit_editingFinished();

private:
    Ui::Widget* ui;
};

#endif // WIDGET_H

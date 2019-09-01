#ifndef FORM_H
#define FORM_H

#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QWidget>
#include <QtGui>
#include <QtNetwork>
namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget* parent = nullptr);
    // Form(QTcpSocket *sock,QWidget *parent = nullptr);
    ~Form();

private slots:
    void on_pushButton_clicked();
    void ret();

private:
    Ui::Form* ui;
    QTcpSocket confd;
};

#endif // FORM_H

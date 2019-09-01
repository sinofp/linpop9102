#ifndef FORM_H
#define FORM_H

#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QWidget>
#include <QtGui>
#include "widget.h"
namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget* parent = nullptr);
    ~Form();

private slots:
    void on_pushButton_clicked();
    void ret();

private:
    Ui::Form* ui;
};

#endif // FORM_H

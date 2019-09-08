#ifndef FORM_H
#define FORM_H

#include "widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPixmap>
#include <QTcpSocket>
#include <QWidget>
#include <QtGui>
namespace Ui {
class Form;
}

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form(QWidget* parent = nullptr);
    ~Form();
    void paintEvent(QPaintEvent*);

private slots:
    void on_pushButton_clicked();
    void ret();

private:
    Ui::Form* ui;
};

#endif // FORM_H

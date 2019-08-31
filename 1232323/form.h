#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include<QMessageBox>
#include<QDebug>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H

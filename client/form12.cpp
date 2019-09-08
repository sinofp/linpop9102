#include "form12.h"
#include "ui_form12.h"
#include <QFile>

Form12::Form12(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Form12)
{
    ui->setupUi(this);
}

Form12::~Form12()
{
    delete ui;
}
void Form12::init(QString file_name)
{
    QFile file(file_name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    ui->textBrowser->setText(QString(t));
    file.close();
}

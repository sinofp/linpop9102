#include "progressdialog.h"
#include "ui_progressdialog.h"

progressdialog::progressdialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::progressdialog)
{
    ui->setupUi(this);
}

progressdialog::~progressdialog()
{
    delete ui;
}
void progressdialog::init_dig(long now, long sum)
{
    ui->progressBar->setMaximum(sum);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(now);
    if (sum == now) {
        ui->pushButton->setEnabled(true);
    } else {
        ui->pushButton->setEnabled(false);
    }
}

void progressdialog::on_pushButton_clicked()
{
    this->close();
}

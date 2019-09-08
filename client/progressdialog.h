#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class progressdialog;
}

class progressdialog : public QDialog {
    Q_OBJECT

public:
    explicit progressdialog(QWidget* parent = nullptr);
    ~progressdialog();

private:
    Ui::progressdialog* ui;
private slots:
    void init_dig(long now, long sum);
    void on_pushButton_clicked();
};

#endif // PROGRESSDIALOG_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(QWidget *parent = nullptr, QTcpSocket *sock=nullptr)
    {
        confd = sock;
    }
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket * confd;
};

#endif // MAINWINDOW_H

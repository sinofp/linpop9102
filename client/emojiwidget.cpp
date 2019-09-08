#include "emojiwidget.h"
#include "ui_emojiwidget.h"
#include <QDebug>

emojiwidget::emojiwidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::emojiwidget)
{
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(getItem_now(int, int)));
}

emojiwidget::~emojiwidget()
{
    delete ui;
}

void emojiwidget::addEmotionItem(QString fileName)
{
    int row = size / ui->tableWidget->columnCount();
    int column = size % ui->tableWidget->columnCount();
    qDebug() << row << ":" << column;

    ui->tableWidget->setItem(row, column, new QTableWidgetItem(QIcon(fileName), ""));
}
void emojiwidget::init_emotion()
{

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 1; i <= 48; i++) {
        QString path = ":/new/prefix1/Source/emoji/";
        path.append(QString::number(i));
        path.append(".png");
        addEmotionItem(path);
        size += 1;
    }
}
void emojiwidget::getItem_now(int row, int column)
{

    long k;
    k = row * 8 + column + 1;

    QString path = ":/new/prefix1/Source/emoji/";
    path.append(QString::number(k));
    path.append(".png");
    emit emit_emoji_path(path);
    this->close();
}

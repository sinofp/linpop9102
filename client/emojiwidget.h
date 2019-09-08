#ifndef EMOJIWIDGET_H
#define EMOJIWIDGET_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class emojiwidget;
}

class emojiwidget : public QWidget {
    Q_OBJECT

public:
    explicit emojiwidget(QWidget* parent = nullptr);
    ~emojiwidget();
    void init_emotion();
    int size = 0;
    QMainWindow* parentt = NULL;
    void addEmotionItem(QString fileName);
signals:
    void emit_emoji_path(QString);

private:
    Ui::emojiwidget* ui;
private slots:
    void getItem_now(int row, int column);
};

#endif // EMOJIWIDGET_H

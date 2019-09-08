#include "gv.h"
#include <QCryptographicHash>
#include <QImage>
#include <QPainter>
#include <QString>

QImage gen_idcon(QString input)
{
    int scale = 200;

    QByteArray hash = QCryptographicHash::hash(input.toLocal8Bit(), QCryptographicHash::Sha256);
    QString digest = hash.toHex();

    int r, g, b;
    r = digest.right(2).toInt(nullptr, 16);
    digest.chop(2);
    g = digest.right(2).toInt(nullptr, 16);
    digest.chop(2);
    b = digest.right(2).toInt(nullptr, 16);
    digest.chop(2);
    QColor color(r, g, b);

    QList<int> grid;
    for (int i = 0; i < 25; i++) {
        grid.push_back(digest.right(2).toInt(nullptr, 16));
        digest.chop(2);
    }
    for (int i = 3; i < 25; i += 5) {
        grid[i] = grid[i - 2];
        grid[i + 1] = grid[i - 3];
    }

    QImage im(QSize(scale * 5, scale * 5), QImage::Format_ARGB32);
    im.fill("white");
    QPainter painter(&im);
    painter.setPen(color);
    painter.setBrush(color);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (grid[j * 5 + i] % 2 == 0) {
                painter.drawRect(i * scale, j * scale, scale, scale);
            }
        }
    }
    im.save(input + ".png");
    return im;
}

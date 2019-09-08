#ifndef HAOYOU_H
#define HAOYOU_H

#include <QDebug>
#include <QGroupBox>
#include <QList>
#include <QToolBox>
#include <QToolButton>
#include <QVBoxLayout>
class FriList : public QToolBox {
    Q_OBJECT
public:
    FriList(QWidget* parent = 0, Qt::WindowFlags f = 0);
    void setLayFri(); //好友页布局
    void setLayGro(); //群组页布局
    void addFriend(int id, QString user, QString icon, QString desc); //添加好友
    void addGroup(int id, QString name, QString icon, QString desc); //添加群组
private:
    QToolButton* toolBtn1;
    QList<QToolButton*> friend_list; //好友列表
    QList<QToolButton*> group_list; //群组列表
    QGroupBox* friendBox; //好友页
    QGroupBox* groupBox; //群组页
    QVBoxLayout* layout_fri; //好友页布局
    QVBoxLayout* layout_group; //群组页布局
};

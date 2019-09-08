#include "frilist.h"
#include <QGroupBox>
#include <QVBoxLayout>

frilist::frilist(QWidget* parent, Qt::WindowFlags f)
    : QToolBox(parent, f)
{
    friendBox = new QGroupBox;
    groupBox = new QGroupBox;

    this->setLayFri();
    this->setLayGro();
}

void frilist::setLayFri()
{
    //好友页布局
    layout_fri = new QVBoxLayout(friendBox);
    layout_fri->setMargin(45);
    layout_fri->setAlignment(Qt::AlignLeft);
    this->addItem((QWidget*)friendBox, QStringLiteral("好友列表"));
}
void frilist::setLayGro()
{
    //群组页布局
    layout_group = new QVBoxLayout(groupBox);
    layout_fri->setMargin(45);
    layout_fri->setAlignment(Qt::AlignLeft);
    this->addItem((QWidget*)groupBox, QStringLiteral("黑名单"));
}

void frilist::addFriend(int id, QString user, QString icon, QString desc)
{
    QToolButton* tempButton = new QToolButton;
    QString iconpath = QString(":/pic/%1.ico").arg(icon);
    QString text = QString("%1 <%2>").arg(user).arg(desc);
    tempButton->setText(text);
    qDebug() << iconpath;
    tempButton->setIcon(QPixmap(iconpath));
    tempButton->setIconSize(QSize(100, 30)); //设置按钮尺寸
    tempButton->setAutoRaise(true); //当鼠标离开时，按钮恢复为弹起状态
    tempButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //设置按钮文字显示在图标旁边
    friend_list.append(tempButton);
    layout_fri->addWidget(friend_list.last());
}

void frilist::addGroup(int id, QString name, QString icon, QString desc)
{
    QToolButton* tempButton = new QToolButton;
    QString iconpath = QString(":/pic/%1.ico").arg(icon);
    QString text = QString("%1 <%2>").arg(name).arg(desc);
    tempButton->setText(text);
    qDebug() << iconpath;
    tempButton->setIcon(QPixmap(iconpath));
    tempButton->setIconSize(QSize(100, 30)); //设置按钮尺寸
    tempButton->setAutoRaise(true); //当鼠标离开时，按钮恢复为弹起状态
    tempButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //设置按钮文字显示在图标旁边
    group_list.append(tempButton);
    layout_group->addWidget(group_list.last());
}

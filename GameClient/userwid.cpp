#include "userwid.h"
#include "ui_userwid.h"
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include "tcpmgr.h"
#include "player.h"
UserWid::UserWid(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::UserWid)
{
    ui->setupUi(this);
    setItemType(ListItemType::CONTACT_USER_ITEM);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&UserWid::customContextMenuRequested,this,&UserWid::slotShowContextMenu);
    connect(this,&UserWid::sigDelFriend,&Player::getInstance(),&Player::slotDelFriend);
}

UserWid::~UserWid()
{
    delete ui;
}

QSize UserWid::sizeHint() const
{
    return QSize(100,60);
}

void UserWid::setInfo(int id,QString name/*, QString level, STATES state*/)
{
    m_id = id;
    m_name = name;
    //m_level = level;
    //m_state = state;
    ui->name_lb->setText(m_name);
    //ui->level_lb->setText(m_level);
    //ui->state_lb->setText(stateString.at(m_state));
}

void UserWid::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"clicked signals";
    if(event->button() == Qt::LeftButton /* && m_state != STATES::OFFLINE*/)
    {
        emit sigOpenChat(m_id);
        return ;
    }
    ListItemBase::mouseReleaseEvent(event);
}

void UserWid::slotShowContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    // 添加菜单项
    QAction *action1 = menu.addAction(tr("删除"));
    connect(action1,&QAction::triggered,[&](){
        //发送删除，包含用户的id
        QJsonObject obj;
        obj["delName"] = m_name;
        emit sigDelFriend(m_id,true);
    });


    QAction *action2 = menu.addAction("邀请游戏");
    connect(action2,&QAction::triggered,[&](){
        emit sigInviteMsg();
    });

    // 在控件上用户点击的位置显示菜单
    menu.exec(mapToGlobal(pos));
}


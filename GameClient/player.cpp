#include "player.h"
#include "tcpmgr.h"
#include "sqlitemgr.h"
#include <QDateTime>

Player::~Player()
{
    SqliteMgr::getInstance().saveApplys(m_applyList);
    clearApplyList();
    clearFriendList();
}

Player::Player()
{
    qDebug()<<"Player construct";
    connect(&TcpMgr::getInstance(),&TcpMgr::sigAddApply,this,&Player::slotAddApply);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigAddFriend,this,&Player::slotAddFriend);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigDelFriend,this,&Player::slotDelFriend);

    connect(&TcpMgr::getInstance(),&TcpMgr::sigLoadApply,[&](const QJsonObject& obj)
    {
        qDebug()<<"sigLoadApply---slotLoadApply";
        int applyId = obj["applyId"].toInt();
        QString applyName = obj["applyId"].toString();
        QString applyTime = obj["applyId"].toString();
        bool finish = obj["finish"].toBool();
        m_applyList.append(new Apply{applyId,applyName,applyTime});
        if(finish)
        {
            SqliteMgr::getInstance().loadApply(m_applyList);
            m_applyload = true;
            std::sort(m_applyList.begin(),m_applyList.end());
            emit sigFlushApplyList();
            if(m_friendsload)
            {

                //emit sigGoGame();
            }

        }
    });

    connect(&TcpMgr::getInstance(),&TcpMgr::sigLoadFriend,[&](const QJsonObject& obj)
    {
        qDebug()<<"sigLoadFriend---slotLoadFriend";
        int friendId = obj["friendId"].toInt();
        QString name = obj["name"].toString();
        //int state = obj["state"].toInt();
        m_friendsList.append(new Friends{friendId,name});
        bool finish = obj["finish"].toBool();
        if(finish)
        {
            std::sort(m_friendsList.begin(),m_friendsList.end());
            emit sigFlushFriendList();
            m_friendsload = true;
            if(m_applyload)
            {
                //emit sigGoGame();
            }
        }
    });
}

void Player::initApplyList()
{
    qDebug()<<"initApplyList";
    clearApplyList();
    //先从服务器下载数据
    QJsonObject obj;
    TcpMgr::getInstance().sigSendData(MSG_LOAD_APPLY,obj);
}

void Player::initFriendList()
{
    qDebug()<<"initFriedList";
    clearFriendList();
    QJsonObject obj;
    TcpMgr::getInstance().sigSendData(MSG_LODA_FRIENDS,obj);
}

//void Player::noticeStateChange()
//{
//    QJsonObject obj;
//    //obj["state"] = m_state;
//    obj["senderId"] = m_id;

//    for(auto& p:m_friendsList)
//    {
//        obj["recvId"] = p->FriendId;
//        emit TcpMgr::getInstance().sigSendData(MSG_STATE_CHANGE,obj);
//    }
//}

//void Player::setState(STATES state)
//{
//    m_state = state;
//    noticeStateChange();
//    qDebug()<<"setState";
//}

void Player::clearFriendList()
{
    for(auto &fri:m_friendsList)
    {
        delete fri;
        fri = nullptr;
    }
    m_friendsList.clear();
}

void Player::clearApplyList()
{
    for(auto &a:m_applyList)
    {
        delete a;
        a = nullptr;
    }
    m_applyList.clear();
}

void Player::slotDealApply(const QString &name, bool f)
{
    Apply *a = nullptr;
    for(int i=m_applyList.size()-1;i>=0;i--)
    {
        if(m_applyList.at(i)->applyName == name)
        {
            qDebug()<<"find user";
            a = m_applyList.at(i);
            m_applyList.removeAt(i);
            break;
        }
    }
    if(f && a!= nullptr)
    {
        qDebug()<<"add Friends";
        QJsonObject obj;
        obj["applyId"] = a->applyId;
        emit TcpMgr::getInstance().sigSendData(MSG_ACCEPT_ADD,obj);
    }
    std::sort(m_applyList.begin(),m_applyList.end());
    emit sigFlushApplyList();
    delete a;
}

void Player::slotAddApply(const QJsonObject &obj)
{
    int applyid = obj["applypid"].toInt();
    QString applyname = obj["name"].toString();
    qDebug()<<applyid;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    bool f = false;
    for(auto ta:m_applyList)
    {
        if(ta->applyId == applyid)
        {
            ta->time = time;
            f = true;
        }
    }
    if(!f)
    {
        Apply *a = new Apply{applyid,applyname,time};
        m_applyList.append(a);
    }
    std::sort(m_applyList.begin(),m_applyList.end());
    emit sigFlushApplyList();
}

void Player::slotAddFriend(const QJsonObject &obj)
{
    int friendId = obj["pid"].toInt();
    QString name = obj["name"].toString();
    //int state = obj["state"].toInt();
    m_friendsList.append(new Friends{friendId,name});
    std::sort(m_friendsList.begin(),m_friendsList.end());
    emit sigFlushFriendList();
}

void Player::slotDelFriend(const int pid,bool isSend)
{
    Friends *f = nullptr;
    for(int i=m_friendsList.size()-1;i>=0;--i)
    {
        if(m_friendsList.at(i)->FriendId == pid)
        {
            f = m_friendsList.at(i);
            m_friendsList.removeAt(i);
            break;
        }
    }
    if(isSend)
    {
        QJsonObject obj;
        obj["pid"] = m_id;
        obj["delId"] = f->FriendId;
        emit TcpMgr::getInstance().sigSendData(MSG_DEL_FRIEND,obj);
    }
    delete f;
    std::sort(m_friendsList.begin(),m_friendsList.end());
    emit sigFlushFriendList();
}

//void Player::slotChangeFriendState(int pid, int state)
//{
//    for(auto& p:m_friendsList)
//    {
//        if(pid = p->FriendId)
//        {
//            p->state = STATES(state);
//        }
//    }
//    std::sort(m_friendsList.begin(),m_friendsList.end());
//    emit sigFlushFriendList();
//}

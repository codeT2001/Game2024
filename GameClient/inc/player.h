#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <inc/global.h>
#include <QList>
#include <QObject>
#include <QMap>
struct Apply{
    int applyId;
    QString applyName;
    QString time;
    bool operator <(const Apply& a) const
    {
        return time > a.time;
    }
};
struct Friends{
    int FriendId;
    QString name;
};
class Player:public QObject
{
    Q_OBJECT
public:
    ~Player();
    static Player& getInstance(){
        static Player instance;
        return instance;
    }
    void initApplyList();
    void initFriendList();
    //void noticeStateChange();
    //void setState(STATES state);
private:
    Player();

    void clearFriendList();
    void clearApplyList();
signals:
    void sigFlushApplyList();
    void sigFlushFriendList();
public slots:
    void slotDealApply(const QString& name,bool f);
    void slotAddApply(const QJsonObject &obj);
    void slotAddFriend(const QJsonObject &obj);
    void slotDelFriend(const int pid,bool isSend);
    //void slotChangeFriendState(int pid,int state);
public:
    int m_id;
    int m_rivalId;
    QString m_name;
    QString m_email;
    QString m_passwd;
    QString m_rival;
    COLOR m_color;
    COLOR r_color;
    //STATES m_state;
    QList<Friends*> m_friendsList;
    QList<Apply*> m_applyList;
    bool m_applyload;
    bool m_friendsload;
    QMap<int,QList<QString>> m_chatMap;

};
#endif // PLAYER_H

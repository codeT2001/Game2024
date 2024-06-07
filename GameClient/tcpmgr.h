#ifndef TCPMGR_H
#define TCPMGR_H


#include <QTcpSocket>
#include "global.h"
#include <QObject>
#include <mutex>
#include <QJsonObject>
class TcpMgr :public QObject
{
    Q_OBJECT
public:
    ~TcpMgr();
    static TcpMgr& getInstance()
    {
        static TcpMgr instance;
        return instance;
    }
    void connectServer(QString host,uint16_t port);
private:
    TcpMgr();
    void initHandlers();
    void handleMsg(MSG_IDS id, int len, QByteArray data);
    bool data2JsonObj(QByteArray& data,QJsonObject& obj);
public slots:
    void slotSendData(MSG_IDS reqId, QJsonObject jsonObj);
    void slotDealMsg();
signals:
    void sigConSuccess(bool isCon);
    void sigLoginSuccess(bool isLogin);
    void sigSendData(MSG_IDS reqId, QJsonObject jsonObj);
    void sigDealMsg();
    void sigGoGame();
    void sigGameStart();
    void sigRecvItem(Item item);
    void sigRecvInfo(QString info);
    void sigRegiserRep(int ok);
    void sigResetRep(int ok);
    void sigGiveUp(bool f);
    void sigHasFind(bool f);            //查找用户
    void sigAddApply(QJsonObject obj);  //添加
    void sigLoadApply(QJsonObject obj); //加载申请
    void sigLoadFriend(QJsonObject obj);//加载朋友列表
    void sigAddFriend(QJsonObject obj); //添加一个朋友
    void sigDelFriend(int pid,bool isSend); //删除一个朋友
    void sigRecvChatInfo(int fid,QString chatName,QString info);
    void sigCantLogin();
    //void sigChangeFriendState(int pid,int state);
private:
    QTcpSocket m_socket;
    QString m_host;
    uint16_t m_port;
    QByteArray m_buffer;
    bool m_recvPending;
    quint16 m_msgid;
    quint16 m_msglen;
    QMap<MSG_IDS, std::function<void(MSG_IDS id,int len, QByteArray data)>>m_handlers;
    std::mutex m_mutex;
};

#endif // TCPMGR_H

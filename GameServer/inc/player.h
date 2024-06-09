/*****************************************************************//**
 * \file   player.h
 * \brief  与客户端进行tcp连接
 * 
 * \author tandingfu
 * \date   June 2024
 *********************************************************************/
#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QTcpSocket>
#include <memory>
#include "global.h"
#include <functional>
#include <qmutex.h>
#include <qjsonobject.h>
#include <qlist.h>
struct Apply {
    int id;
    QString name;
    QString time;
};
class Player:public QObject
{
    Q_OBJECT
public:
    STATES m_state;
    explicit Player(QTcpSocket* socket);
    //STATES getState();
    int getPid();
    //void setState(STATES state);
    QString getName();
    QString getEmail();
    QString getPwd();

//    bool send();
//    bool send();
//    void updateHeartbeat();
//    bool isHeartbeatTimeout();

signals:
    void sigDisConnected(); //向TcpMgr 发送用户断开信号
    void sigSendTcpMgr(MSG_IDS id, QJsonObject obj);   //向TcpMgr 发送消息
    void sigDealMsg();  //分包处理信号
    void sigSendData(MSG_IDS id,QJsonObject obj);  //向用户发送消息
private slots:
    void slotDealMsg();
    void slotSendData(MSG_IDS id, QJsonObject obj);
private:
    void handleMsg(MSG_IDS id, int len, QByteArray data);
    void initHandlers();
    bool data2JsonObj(QByteArray& data, QJsonObject& obj);
private:
    using fn = std::function<void(MSG_IDS id, int len, QByteArray data)>;
    std::shared_ptr<QTcpSocket> m_socket;
    QByteArray m_buffer;
    bool m_recvPending;
    quint16 m_msgid;
    quint16 m_msglen;
    QMap <MSG_IDS,fn> m_handlers;
    QMutex m_mutex;
    //STATES m_state;
    int m_pid;
    QString m_name;
    QString m_email;
    QString m_passwd;
};

#endif // PLAYER_H

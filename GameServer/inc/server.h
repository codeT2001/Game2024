/*****************************************************************//**
 * \file   server.h
 * \brief  处理新的客户端连接和客户端与客户端之间的通信
 * 
 * \author tandingfu
 * \date   June 2024
 *********************************************************************/
#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include "player.h"
#include "QMap"
#include <memory>
#include <functional>
#include <qqueue.h>
#include <mutex>
class Server:public QTcpServer
{
    Q_OBJECT
public:
    static Server& getInstance()
    {
        static Server instance;
        return instance;
    }
    bool start(uint16_t port);
    ~Server();
signals:
    void sigAddMatchPlayer(int pid);
    void sigStartMatch();
    void sigStartGame(int p1, int p2);

private:
    Server(QObject *parent = nullptr);
    void checkMap();
    void initHandlers();
    void handleMsg(Player* p,MSG_IDS id, QJsonObject& obj);

private slots:
    void slotRecvPlayerMsg(MSG_IDS id, QJsonObject obj);
    void slotStartMatch();
    void slotAddMatchPlayer(int pid);
    void slotStartGame(int p1,int p2);

private:
    using fn = std::function<void(Player* p, MSG_IDS id,QJsonObject& obj)>;
    QMap<int,Player*> m_players;
    QMap <MSG_IDS, fn> m_handlers;
    QQueue<int> m_matchQue; 
    std::mutex m_mutex;     //管理匹配队列
    int m_matchs;
};

#endif // SERVER_H

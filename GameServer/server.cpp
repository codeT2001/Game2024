#include "server.h"
#include <iostream>
#include "mysqlmgr.h"
#include <qdatetime.h>
bool Server::start(uint16_t port)
{
    std::cout << "server listen to " << port << std::endl;
    return listen(QHostAddress::Any,port);
}

Server::~Server()
{
    for (auto& p : m_players)
    {
        if (p == nullptr)
        {
            std::cout << "player is nullptr" << std::endl;
        }
        else
        {
            std::cout << "p not is nullptr" << std::endl;
            p->deleteLater();
        }
    }
}

Server::Server(QObject *parent):m_matchs(0)
{
    Q_UNUSED(parent);
    connect(this,&QTcpServer::newConnection,[this](){
        if(hasPendingConnections())
        {
            std::cout << "new Connection" << std::endl;
            QTcpSocket *socket = nextPendingConnection();
            if(socket->isValid())
            {
                Player* p = new Player(socket);
                connect(p, &Player::sigDisConnected, [p,this]() {
                    //当用户断开后删除指针p,在Player类中删除指针socket
                    if (m_players.find(p->getPid()) != m_players.end())
                    {
                        m_players.remove(p->getPid());
                    }
                    p->deleteLater();
                });
                connect(p, &Player::sigSendTcpMgr, this, &Server::slotRecvPlayerMsg);
            }
        }
    });

    //向匹配队列里面添加玩家
    connect(this, &Server::sigAddMatchPlayer, this, &Server::slotAddMatchPlayer);
    //开始匹配
    connect(this, &Server::sigStartMatch, this, &Server::slotStartMatch);
    //匹配结束，通知开始游戏
    connect(this, &Server::sigStartGame, this, &Server::slotStartGame);

    initHandlers();
}

void Server::slotRecvPlayerMsg(MSG_IDS id, QJsonObject obj)
{
    Player* p = (Player*)sender();
    std::cout << p << std::endl;
    handleMsg(p, id, obj);
}

void Server::checkMap()
{
    for (auto& p : m_players)
    {
        if (p == nullptr)
        {
            std::cout << "player is nullptr" << std::endl;
        }
        else
        {
            std::cout << "p not is nullptr" << std::endl;
            p->deleteLater();
        }
    }
}

void Server::initHandlers()
{
    std::cout << "Server ";
    m_handlers[MSG_IDS::MSG_LOGIN_SUCCESS] = [&](Player* p, MSG_IDS id, QJsonObject& obj){
        int pid = obj["pid"].toInt();
        auto np = m_players.find(pid);
        //不是第一次登录，更新用户相应信息
        if (np != m_players.end())
        {
            std::cout << "不是第一次登录" << std::endl;
        }
        m_players[pid] = p;
    };

    m_handlers[MSG_IDS::MSG_MATCH] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int pid = obj["pid"].toInt();
        std::cout << pid <<"号玩家开始匹配" << std::endl;
        emit sigAddMatchPlayer(pid);
    };

    m_handlers[MSG_IDS::MSG_MOVE] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int pid = obj["rivalId"].toInt();
        emit m_players[pid]->sigSendData(MSG_MOVE,obj);
    };
    
    m_handlers[MSG_IDS::MSG_CHAT_GAME] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int pid = obj["rivalId"].toInt();
        emit m_players[pid]->sigSendData(MSG_CHAT_GAME, obj);
    };

    m_handlers[MSG_IDS::MSG_CANCEL_MATCH] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        std::cout << "OPT is " << id << "\n";
        int pid = obj["pid"].toInt();
        std::lock_guard<std::mutex> lock(m_mutex);
        m_matchs--;
        m_players[pid]->m_state = ONLINE;
        //m_players[pid]->setState(ONLINE);
    };

    m_handlers[MSG_IDS::MSG_GIVEUP] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int rivalId = obj["rivalId"].toInt();
        obj["rivalId"] = p->getPid();
        emit m_players[rivalId]->sigSendData(MSG_GIVEUP,obj);
    };

    //查到用户，给用户发申请
    m_handlers[MSG_IDS::MSG_ADD_PLAYER] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int applypid = obj["applypid"].toInt(); //申请人
        int tpid = obj["tpid"].toInt(); //申请对象
        std::cout << "tpid = " << tpid << std::endl;
        if (m_players.find(tpid) != m_players.end() && m_players[tpid] != nullptr)
        {
            std::cout << "plaers is onlinne" << std::endl;
            obj["name"] = m_players[applypid]->getName();
            emit m_players[tpid]->sigSendData(MSG_ADD_APPLY, obj);
        }
        else
        {
            std::cout << "plaers is offlinne" << std::endl;
            QString applyName = m_players[applypid]->getName();
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
            MysqlMgr::getInstance().addApply(tpid, applypid,applyName,time);
        }
    };

    m_handlers[MSG_IDS::MSG_ACCEPT_ADD] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int pid = obj["recvid"].toInt();
        if (m_players.find(pid) != m_players.end() && m_players[pid] != nullptr)
        {
            emit m_players[pid]->sigSendData(MSG_ACCEPT_ADD, obj);
        }
    };

    m_handlers[MSG_IDS::MSG_DEL_FRIEND] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        int pid = obj["pid"].toInt();
        int delId = obj["delId"].toInt();
        MysqlMgr::getInstance().deleteFriend(pid, delId);
        if (m_players.find(delId) != m_players.end() && m_players[delId] != nullptr)
        {
            emit m_players[delId]->sigSendData(MSG_DEL_FRIEND,obj);
        }
    };
    m_handlers[MSG_IDS::MSG_CHAT_INFO] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        
        int pid = obj["chatId"].toInt();
        int senderId = obj["senderId"].toInt();
        obj["chatName"] = m_players[senderId]->getName();
        emit m_players[pid]->sigSendData(MSG_CHAT_INFO, obj);
    };
 /*   m_handlers[MSG_IDS::MSG_STATE_CHANGE] = [&](Player* p, MSG_IDS id, QJsonObject& obj) {
        if(m_players.find())
        emit m_players[pid]->sigSendData(MSG_CHAT_INFO, obj);
        };*/
}

void Server::handleMsg(Player *p,MSG_IDS id, QJsonObject& obj)
{
    auto iter = m_handlers.find(id);
    if (iter == m_handlers.end())
    {
        std::cout << "not fond id [" << id << "] from handle" << std::endl;
        return;
    }
    iter.value()(p, id, obj);
}

void Server::slotStartMatch()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    int p1 = -1 ,p2 = -1;
    if (m_matchs < 2)
    {
        return;
    }
    int d = m_matchs % 2;
    while ((m_matchs > d) && (!m_matchQue.isEmpty()))
    {
        std::cout << m_matchs << std::endl;
        int pid = m_matchQue.front();
        m_matchQue.pop_front();
        //std::cout << m_players[pid]->getPid()<<' ' << m_players[pid]->getState() << std::endl;
        //std::cout << "MATCHING is " << MATCHING << std::endl;
        if (m_players[pid]->m_state!=MATCHING || pid == p1)
        {
            continue;
        }
        if (p1 < 0)
        {
            p1 = pid;
            m_matchs--;
        }
        else if(p2<0)
        {
            p2 = pid;
            m_matchs--;
        }
        if (p1 > 0 && p2 > 0)
        {
            m_players[p1]->m_state = PLAYING;
            m_players[p2]->m_state = PLAYING;
            emit sigStartGame(p1, p2);
            p1 = -1;
            p2 = -1;
        }
    }
    lock.unlock();
    
}

void Server::slotAddMatchPlayer(int pid)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_matchQue.push_back(pid);
    m_matchs += 1;
    lock.unlock();
    emit sigStartMatch();
    
}
void Server::slotStartGame(int p1, int p2)
{
    if (p1 < 0 || p2 < 0) return;
    //给双方的客户端发送匹配成功的消息，游戏开始，
    //数据包里面有对方的信息，对方的pid，自己的棋子颜色
    QJsonObject obj;
    obj["rivalId"] = p2;
    obj["rival"] = m_players[p2]->getName();
    obj["color"] = COLOR::BLACK;
    obj["rivalC"] = COLOR::WHITE;
    emit m_players[p1]->sigSendData(MSG_IDS::MSG_MACTH_SUCCESS,obj);
    obj["rivalId"] = p1;
    obj["rival"] = m_players[p1]->getName();
    obj["color"] = COLOR::WHITE;
    obj["rivalC"] = COLOR::BLACK;
    emit m_players[p2]->sigSendData(MSG_IDS::MSG_MACTH_SUCCESS, obj);
}

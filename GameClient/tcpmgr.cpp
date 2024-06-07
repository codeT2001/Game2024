#include "tcpmgr.h"
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <iostream>
#include "player.h"
TcpMgr::~TcpMgr()
{

}

void TcpMgr::connectServer(QString host, uint16_t port)
{
    qDebug()<< "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    m_host = host;
    m_port = port;
    m_socket.connectToHost(host, port);
}

TcpMgr::TcpMgr():m_host(""),m_port(0)
    ,m_recvPending(false),m_msgid(0),m_msglen(0)
{
    QObject::connect(&m_socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to server!";
        // 连接建立后发送消息
        emit sigConSuccess(true);
    });

    QObject::connect(&m_socket, &QTcpSocket::readyRead, [&]() {
        m_buffer.append(m_socket.readAll());
        emit sigDealMsg();
    });
    connect(this, &TcpMgr::sigDealMsg, this, &TcpMgr::slotDealMsg);
    connect(this,&TcpMgr::sigSendData, this, &TcpMgr::slotSendData);
    //5.15 之后版本
    QObject::connect(&m_socket,
                     QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                     [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << m_socket.errorString();
        emit sigConSuccess(false);
    });

    // 处理连接断开
    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });
    //连接发送信号用来发送数据

    //注册消息
    initHandlers();
}

void TcpMgr::initHandlers()
{
    //获取登录请求的回包
    m_handlers[MSG_IDS::MSG_LOGIN_REP] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        int find = obj["find"].toInt();
        if(find == MSG_FIND_SUCCESS)
        {
            //服务器成功查询到用户
            Player& p = Player::getInstance();
            p.m_id = obj["pid"].toInt();
            p.m_name = obj["name"].toString();
            p.m_passwd = obj["passwd"].toString();
            p.m_email = obj["email"].toString();
            //p.m_state = STATES(obj["state"].toInt());
            //给mainwindow发送登录信号
            emit sigGoGame();
        }
        else
        {
            //没有查询到用户，不允许登录
            emit sigCantLogin();
        }

    };
    //匹配成功的回复
    m_handlers[MSG_IDS::MSG_MACTH_SUCCESS] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        Player& p = Player::getInstance();
        p.m_color = COLOR(obj["color"].toInt());
        p.m_rival = obj["rival"].toString();
        p.m_rivalId = obj["rivalId"].toInt();
        p.r_color  = COLOR(obj["rivalC"].toInt());
        //p.setState(PLAYING);
        emit sigGameStart();
    };
    //对方棋子操作的回复
    m_handlers[MSG_IDS::MSG_MOVE] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }

        COLOR c = COLOR(obj["color"].toInt());
        int x = obj["posx"].toInt();
        int y = obj["posy"].toInt();
        emit sigRecvItem(Item{QPoint(x,y),c});
    };
    //对方游戏中的聊天
    m_handlers[MSG_IDS::MSG_CHAT_GAME] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        QString info = obj["info"].toString();
        emit sigRecvInfo(info);
    };
    //注册的回包
    m_handlers[MSG_IDS::MSG_REGISTER_REP] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        int ok = obj["rep"].toInt();
        emit sigRegiserRep(ok);
    };
    //重置密码回包
    m_handlers[MSG_IDS::MSG_RESET_REP] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        int ok = obj["rep"].toInt();
        emit sigResetRep(ok);
    };
    //接收到投降的消息
    m_handlers[MSG_IDS::MSG_GIVEUP] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        emit sigGiveUp(false);
        //Player::getInstance().setState(ONLINE);
    };
    //接收到寻找用户的消息
    m_handlers[MSG_IDS::MSG_ADD_PLAYER] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        bool f = obj["find"].toBool();
        emit sigHasFind(f);
    };
    //接收到好友申请的消息
    m_handlers[MSG_IDS::MSG_ADD_APPLY] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        emit sigAddApply(obj);
    };
    //下载申请数据
    m_handlers[MSG_IDS::MSG_LOAD_APPLY] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        emit sigLoadApply(obj);
    };
    //下载好友数据
    m_handlers[MSG_IDS::MSG_LODA_FRIENDS] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        emit sigLoadFriend(obj);
    };
    //在线用户添加好友
    m_handlers[MSG_IDS::MSG_ACCEPT_ADD] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        emit sigAddFriend(obj);
    };
    //在线用户收到被删除信息
    m_handlers[MSG_IDS::MSG_DEL_FRIEND] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        int pid = obj["pid"].toInt();
        emit sigDelFriend(pid,false);
    };

    //好友的聊天
    m_handlers[MSG_IDS::MSG_CHAT_INFO] = [&](MSG_IDS id,int len,QByteArray data){
        QJsonObject obj;
        if(!data2JsonObj(data,obj))
        {
            return ;
        }
        QString info = obj["info"].toString();
        int fid = obj["senderId"].toInt();
        QString chatName = obj["chatName"].toString();
        emit sigRecvChatInfo(fid,chatName,info);
    };

//    //好友状态的改变
//    m_handlers[MSG_IDS::MSG_STATE_CHANGE] = [&](MSG_IDS id,int len,QByteArray data){
//        QJsonObject obj;
//        if(!data2JsonObj(data,obj))
//        {
//            return ;
//        }

//        int fid = obj["senderId"].toInt();
//        int state = obj["state"].toInt();
//        emit sigChangeFriendState(fid,state);
//    };
}

void TcpMgr::handleMsg(MSG_IDS id, int len, QByteArray data)
{
    auto find_iter =  m_handlers.find(id);
    if(find_iter == m_handlers.end()){
        qDebug()<< "not found id ["<< id << "] to handle";
        return ;
    }
    find_iter.value()(id,len,data);
}

void TcpMgr::slotSendData(MSG_IDS reqId, QJsonObject jsonObj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    uint16_t id = reqId;
    jsonObj["id"] = id;
    QByteArray databytes = QJsonDocument(jsonObj).toJson();
    // 计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(databytes.size());
    // 创建一个QByteArray用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);
    // 写入ID和长度
    out << id << len;
    // 添加字符串数据
    block.append(databytes);
    qDebug() << "Message ID:" << id << ", Length:" << len;
    qDebug()<<"send body is "<<QString(block);
    // 发送数据
    m_socket.write(block);
    m_socket.flush();
}

void TcpMgr::slotDealMsg()
{
    QDataStream stream(&m_buffer, QIODevice::ReadOnly);
    //设置Qt版本 Qt_6_2_3
    stream.setVersion(QDataStream::Qt_6_2);
    if (!m_recvPending)
    {
        // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
        if (m_buffer.size() < static_cast<int>(sizeof(quint16) * 2))
        {
            return; // 数据不够，等待更多数据
        }
        // 预读取消息ID和消息长度，但不从缓冲区中移除
        stream >> m_msgid >> m_msglen;
        //将buffer 中的前四个字节移除
        m_buffer = m_buffer.remove(0, sizeof(quint16) * 2);
        // 输出读取的数据
        qDebug() << "Message ID:" << m_msgid << ", Length:" << m_msglen;

    }
    //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
    if (m_buffer.size() < m_msglen)
    {
        m_recvPending = true;
        return;
    }
    m_recvPending = false;
    // 读取消息体
    QByteArray messageBody = m_buffer.mid(0, m_msglen);
    qDebug() << "receive body msg is " << messageBody;
    m_buffer.remove(0, m_msglen);
    handleMsg(MSG_IDS(m_msgid), m_msglen, messageBody);
    emit sigDealMsg();
}

bool TcpMgr::data2JsonObj(QByteArray& data,QJsonObject& obj)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull())
    {
        std::cout << "failed to create QJsonDocument.";
        return false;
    }
    obj = jsonDoc.object();
    return true;
}

#include "player.h"
#include <iostream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <mysqlmgr.h>
Player::Player(QTcpSocket* socket)
	:m_recvPending(false),m_msgid(0),m_msglen(0),m_state(ONLINE)
{
	m_socket.reset(socket);

	connect(m_socket.get(), &QTcpSocket::disconnected, [&]() {
		std::cout << "socket disconnected " << std::endl;
		//m_state = OFFLINE;
		//MysqlMgr::getInstance().updateState(m_pid,m_state);
		//向Tcp发送玩家断开消息
		emit sigDisConnected();
		});

	connect(m_socket.get(), &QTcpSocket::readyRead, [&]() {
		// 当有数据可读时，读取所有数据并追加到缓冲区
		m_buffer.append(m_socket->readAll());
		emit sigDealMsg();
	});

	connect(this, &Player::sigSendData, this, &Player::slotSendData);

	connect(this, &Player::sigDealMsg, this, &Player::slotDealMsg);

	initHandlers();
}

//STATES Player::getState()
//{
//	return STATES(m_state);
//}

int Player::getPid()
{
	return m_pid;
}

//void Player::setState(STATES state)
//{
//	std::cout << state << std::endl;
//	m_state = state;
//}

QString Player::getName()
{
	return m_name;
}

QString Player::getEmail()
{
	return m_email;
}

QString Player::getPwd()
{
	return m_passwd;
}

void Player::slotDealMsg()
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
		std::cout << "Message ID:" << m_msgid << ", Length:" << m_msglen << std::endl;
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
	std::cout << "receive body msg is " << messageBody.toStdString() << std::endl;

	m_buffer.remove(0, m_msglen);
	handleMsg(MSG_IDS(m_msgid), m_msglen, messageBody);
	emit sigDealMsg();
}

void Player::slotSendData(MSG_IDS id, QJsonObject obj)
{
	quint16 msgid = id;
	obj["id"] = id;
	QByteArray databytes = QJsonDocument(obj).toJson();
	// 计算长度（使用网络字节序转换）
	quint16 len = static_cast<quint16>(databytes.size());
	// 创建一个QByteArray用于存储要发送的所有数据
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	// 设置数据流使用网络字节序
	out.setByteOrder(QDataStream::BigEndian);
	// 写入ID和长度
	out << msgid << len;
	// 添加字符串数据
	block.append(databytes);
	std::cout << "Message ID:" << id << ", Length:" << len << std::endl;
	std::cout << "send body msg is " << block.toStdString() << std::endl;
	// 发送数据
	m_socket->write(block);
	m_socket->flush();
}

void Player::handleMsg(MSG_IDS id, int len, QByteArray data)
{
	auto iter = m_handlers.find(id);
	if (iter == m_handlers.end())
	{
		std::cout << "not fond id [" << id << "] from handle" << std::endl;
		return;
	}
	iter.value()(id, len, data);
}

void Player::initHandlers()
{
	//测试
	m_handlers[MSG_IDS::MSG_HW] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject jsonObj;
		if (!data2JsonObj(data, jsonObj))
		{
			return;
		}
		std::cout << jsonObj["data"].toString().toStdString() << std::endl;
	};
	
	//处理获取验证码请求
	m_handlers[MSG_IDS::MSG_GET_CODE] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject jsonObj;
		if (!data2JsonObj(data, jsonObj))
		{
			return;
		}
	};

	//处理登录请求
	m_handlers[MSG_IDS::MSG_LOGIN] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		QString email = obj["email"].toString();
		QString passwd = obj["passwd"].toString();
		PLAYER* p = new PLAYER;
		memset(p, 0, sizeof(p));
		bool findp = MysqlMgr::getInstance().findPlayer(email,passwd,p);
		if (findp)
		{
			//将玩家信息发给客户端，通知客户端登录
			obj["pid"] = p->id;
			obj["name"] = p->name;
			obj["find"] = MSG_IDS::MSG_FIND_SUCCESS;
			m_pid = p->id;
			m_name = p->name;
			m_passwd = p->passwd;
			m_email = p->email;
			//m_state = p->state;
		}
		else
		{
			obj["find"] = MSG_IDS::MSG_FIND_FAILED;
		}
		delete p;
		emit sigSendData(MSG_IDS::MSG_LOGIN_REP, obj);
	};

	//处理登录成功信息
	m_handlers[MSG_IDS::MSG_LOGIN_SUCCESS] = [this](MSG_IDS id, int len, QByteArray data){
		std::cout << "handle id is " << id << std::endl;
		//如果玩家登录成功，更新自己的信息，通知Server将玩家填入到队列中
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		obj["pid"] = m_pid;
		//m_state = ONLINE;
		emit sigSendTcpMgr(MSG_IDS::MSG_LOGIN_SUCCESS,obj);
	};

	//处理匹配请求
	m_handlers[MSG_IDS::MSG_MATCH] = [this](MSG_IDS id, int len, QByteArray data){
		
		std::cout << "handle id is " << id << std::endl;
		//不能自己和自己匹配
		/*if (m_state == STATES::MATCHING)
		{
			return;
		}*/
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		m_state = STATES::MATCHING;
		emit sigSendTcpMgr(MSG_IDS::MSG_MATCH, obj);
	};
	
	//处理发送过来的棋子移动信息,通知Server发送给对手
	m_handlers[MSG_IDS::MSG_MOVE] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		emit sigSendTcpMgr(MSG_IDS::MSG_MOVE, obj);
	};

	//处理游戏里面的交流请求
	m_handlers[MSG_IDS::MSG_CHAT_GAME] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		emit sigSendTcpMgr(MSG_IDS::MSG_CHAT_GAME, obj);
	};

	//处理取消匹配请求 TODO
	m_handlers[MSG_IDS::MSG_CANCEL_MATCH] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		m_state = STATES::ONLINE;
		//通知所有好友，更改好友列表状态
		emit sigSendTcpMgr(MSG_IDS::MSG_CANCEL_MATCH, obj);
	};

	//处理注册请求
	m_handlers[MSG_IDS::MSG_REGISTER] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		//数据库进行注册
		QString name = obj["name"].toString();
		QString email = obj["email"].toString();
		QString passwd = obj["passwd"].toString();
		QJsonObject nobj;
		if (MysqlMgr::getInstance().addPlayer(name, email, passwd))
		{
			nobj["rep"] = 1;			//返回成功信息
		}
		else
		{
			nobj["rep"] = 0;			//返回失败信息
		}
		emit sigSendData(MSG_REGISTER_REP, nobj);
	};

	//处理重置密码请求
	m_handlers[MSG_IDS::MSG_RESET] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		//数据库进行重置
		QJsonObject nobj;
		QString email = obj["email"].toString();
		QString passwd = obj["newpwd"].toString();
		if (MysqlMgr::getInstance().resetPassword(email, passwd))
		{
			nobj["rep"] = 1;
		}
		else
		{
			nobj["rep"] = 0;
		}
		emit sigSendData(MSG_RESET_REP, nobj);
	};

	//处理认输请求
	m_handlers[MSG_IDS::MSG_GIVEUP] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		emit sigSendTcpMgr(MSG_GIVEUP, obj);
	};

	//处理添加好友请求
	m_handlers[MSG_IDS::MSG_ADD_PLAYER] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		QString name = obj["name"].toString();
		QJsonObject tobj;
		int tpid = -1;
		if (MysqlMgr::getInstance().findPlayerByName(name, tpid))
		{
			std::cout << "find user in db\n";
			//找到用户，开始执行添加
			obj["applypid"] = m_pid;	//申请人是自己
			obj["tpid"] = tpid;			//对象id
			//让server添加，用户在线
			emit sigSendTcpMgr(MSG_ADD_PLAYER, obj);
			tobj["find"] = true;
		}
		else
		{
			tobj["find"] = false;
		}
		emit sigSendData(MSG_ADD_PLAYER,tobj);

	};
	//同意添加好友请求
	m_handlers[MSG_IDS::MSG_ACCEPT_ADD] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		int applyId = obj["applyId"].toInt();
		PLAYER* p = new PLAYER{};
		QJsonObject o;
		MysqlMgr::getInstance().addFriends(m_pid,applyId);
		MysqlMgr::getInstance().findPlayerById(applyId,p);
		if (p)	//获取申请人用户的信息
		{
			o["pid"] = p->id;
			o["name"] = p->name;
			//o["state"] = p->state;
			emit sigSendData(MSG_ACCEPT_ADD, o);
		}
		MysqlMgr::getInstance().addFriends(applyId,m_pid);
		o["pid"] = m_pid;
		o["name"] = m_name;
		//o["state"] = m_state;
		o["recvid"] = applyId;
		emit sigSendTcpMgr(MSG_ACCEPT_ADD, o);//让server查询申请人是否在线，发送信息
		delete p;
	};
	//客户端下载添加用户申请
	m_handlers[MSG_IDS::MSG_LOAD_APPLY] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		int pid = obj["applyId"].toInt();
		QList<Apply> list;
		MysqlMgr::getInstance().loadApplys(m_pid,list);
		for (const auto& a : list)
		{
			obj["applyId"] = a.id;
			obj["applyName"] = a.name;
			obj["applyTime"] = a.time;
			obj["finish"] = false;
			if (a.id == list.back().id)
			{
				obj["finish"] = true;
			}
			emit sigSendData(MSG_LOAD_APPLY,obj);
		}
	};
	//客户端下载好友申请
	m_handlers[MSG_IDS::MSG_LODA_FRIENDS] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		QList<PLAYER> list;
		MysqlMgr::getInstance().loadFriends(m_pid,list);
		for (const auto& a : list)
		{
			obj["friendId"] = a.id;
			obj["name"] = a.name;
			//obj["state"] = a.state;
			obj["finish"] = false;
			if (a.id == list.back().id)
			{
				obj["finish"] = true;
			}
			emit sigSendData(MSG_LODA_FRIENDS, obj);
		}
	};
	//删除好友
	m_handlers[MSG_IDS::MSG_DEL_FRIEND] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		emit sigSendTcpMgr(MSG_DEL_FRIEND, obj);
	};
	//好友聊天
	m_handlers[MSG_IDS::MSG_CHAT_INFO] = [this](MSG_IDS id, int len, QByteArray data) {
		std::cout << "handle id is " << id << std::endl;
		QJsonObject obj;
		if (!data2JsonObj(data, obj))
		{
			return;
		}
		emit sigSendTcpMgr(MSG_IDS::MSG_CHAT_INFO, obj);
	};
	////状态改变
	//m_handlers[MSG_IDS::MSG_STATE_CHANGE] = [this](MSG_IDS id, int len, QByteArray data) {
	//	std::cout << "handle id is " << id << std::endl;
	//	QJsonObject obj;
	//	if (!data2JsonObj(data, obj))
	//	{
	//		return;
	//	}
	//	emit sigSendTcpMgr(MSG_IDS::MSG_STATE_CHANGE, obj);
	//	};
}

bool Player::data2JsonObj(QByteArray& data,QJsonObject& obj)
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


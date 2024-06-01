#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "msgnode.h"
#include <queue>
#include <mutex>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;
class CServer;
class CSession:public std::enable_shared_from_this<CSession>
{

public:
	CSession(boost::asio::io_context& io_context,CServer* server);
	tcp::socket& getSocket();
	std::string& getUuid();
	void start();
	void close();
	~CSession();
	void send(const char* msg, short max_length, short msg_id);
	void send(std::string msg, short msg_id);

private:
	void handleWrite(const boost::system::error_code& error
		,std::shared_ptr<CSession>self);
	boost::asio::io_context& _io_context;
	CServer* _server;
	tcp::socket _socket;
	std::string _uuid;
	bool _b_close;
	//std::mutex _mutex;
	std::mutex _send_lock;
	std::queue<std::shared_ptr<SendNode>> _send_que;
	std::shared_ptr<RecvNode> _recv_msg_node;
	std::shared_ptr<MsgNode> _recv_head_node;
};
class LogicNode {
public:
	LogicNode(std::shared_ptr<CSession>, std::shared_ptr<RecvNode>);
	std::shared_ptr<CSession> _session;
	std::shared_ptr<RecvNode> _recvnode;
};


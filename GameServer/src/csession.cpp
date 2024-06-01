#include "csession.h"
#include "cserver.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "const.h"
#include "logicsystem.h"

CSession::CSession(boost::asio::io_context& io_context, CServer* server)
	:_io_context(io_context),_server(server)
	,_socket(io_context),_b_close(false)
{
	boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

boost::asio::ip::tcp::socket& CSession::getSocket()
{
	return _socket;
}

std::string& CSession::getUuid()
{
	return _uuid;
}

void CSession::start()
{
	auto shared_this = shared_from_this();
	//开启协程
	co_spawn(_io_context, [=]()->boost::asio::awaitable<void>{
		try {
			for (; !_b_close;)
			{
				_recv_head_node->clear();
				std::size_t n = co_await boost::asio::async_read(_socket, boost::asio::buffer(_recv_head_node->_data,
					HEAD_TOTAL_LEN), boost::asio::use_awaitable);
				if (n == 0)
				{
					std::cout << "receive peer closed" << std::endl;
					close();
					_server->clearSession(_uuid);
					co_return;
				}

				//获取头部MSGID数据
				short msg_id = 0;
				memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
				//网络字节序转为本地
				msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
				std::cout << "msg_id is " << msg_id << std::endl;
				if(msg_id > MAX_LENGTH)
				{
					std::cout << "invalid msg id is " << msg_id << std::endl;
					close();
					_server->clearSession(_uuid);
					co_return;
				}

				short msg_len = 0;
				memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN
					, HEAD_DATA_LEN);
				msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
				std::cout << "msg len is " << msg_len << std::endl;

				if (msg_len > MAX_LENGTH)
				{
					std::cout << "invalid msg len is " << msg_len << std::endl;
					close();
					_server->clearSession(_uuid);
					co_return;
				}

				_recv_msg_node = std::make_shared<RecvNode>(msg_len, msg_id);
				//读出包体
				n = co_await boost::asio::async_read(_socket,boost::asio::buffer(_recv_msg_node->_data
				,_recv_msg_node->_total_len),use_awaitable);

				if (n == 0)
				{
					std::cout << "receive peer closed" << std::endl;
					close();
					_server->clearSession(_uuid);
					co_return;
				}

				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				std::cout << "receive data is" << _recv_msg_node->_data << std::endl;
				//投递逻辑线程处理
				LogicSystem::getInstance().postMsgToQue(std::make_shared<LogicNode>
					(shared_from_this(),_recv_msg_node));
			}
		}
		catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
			close();
			_server->clearSession(_uuid);
		}
	},boost::asio::detached);

}

void CSession::close()
{
	_socket.close();
	_b_close = true;
}

CSession::~CSession()
{
	try {
		std::cout << "~CSession destruct" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "exception is " << e.what() << std::endl;
	}
}

void CSession::send(const char* msg, short max_length, short msg_id)
{
	std::unique_lock<std::mutex> lock(_send_lock);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE)
	{
		std::cout << "session : " << _uuid << " send que fulled,size is " 
			<< MAX_SENDQUE << std::endl;
		return;
	}
	_send_que.push(std::make_shared<SendNode>(msg, max_length, msg_id));
	if (send_que_size > 0)
	{
		return;
	}
	auto msgnode = _send_que.front();
	lock.unlock();
	boost::asio::async_write(_socket,boost::asio::buffer(msgnode->_data,msgnode->_total_len),
		std::bind(&CSession::handleWrite,this,std::placeholders::_1,shared_from_this()));
}

void CSession::send(std::string msg, short msg_id)
{
	send(msg.c_str(), msg.length(), msg_id);
}

void CSession::handleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> self)
{
	try {
		if (!error)
		{
			std::unique_lock<std::mutex> lock(_send_lock);
			_send_que.pop();
			if (!_send_que.empty())
			{
				auto msgnode = _send_que.front();
				lock.unlock();
				boost::asio::async_write(_socket,boost::asio::buffer(msgnode->_data,msgnode->_total_len)
					,std::bind(&CSession::handleWrite,this,std::placeholders::_1,shared_from_this()));
			}
		}
		else
		{
			std::cout << "handle write failed,error is " <<error.what() << std::endl;
			close();
			_server->clearSession(_uuid);
		}
	}
	catch (std::exception& e) {
		std::cout << "exception is " << e.what() << std::endl;
		close();
		_server->clearSession(_uuid);
	}
}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode>recvnode)
	:_session(session),_recvnode(recvnode)
{

}

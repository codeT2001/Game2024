#pragma once
#include <memory>
#include <map>
#include <mutex>
#include <boost/asio.hpp>
#include "csession.h"
using boost::asio::ip::tcp;

class CServer
{
public:
	CServer(boost::asio::io_context& context,short port);
	~CServer();
	void clearSession(std::string);
private:
	void handleAccept(std::shared_ptr<CSession>,const boost::system::error_code error);
	void startAccept();
	boost::asio::io_context& _io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map < std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex;

};


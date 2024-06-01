#include "csession.h"
#include "cserver.h"
#include "asioioservicepool.h"
#include <iostream>
CServer::CServer(boost::asio::io_context& context, short port)
	:_io_context(context),_port(port)
	,_acceptor(context,tcp::endpoint(tcp::v4(),port))
{
	startAccept();
}

CServer::~CServer()
{
}

void CServer::clearSession(std::string uuid)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_sessions.erase(uuid);
}

void CServer::handleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code error)
{
	if (!error)
	{
		new_session->start();
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions[new_session->getUuid()] = new_session;
	}
	else
	{
		std::cout << "session accpet failed error is" << error.what() << std::endl;
	}
	startAccept();
}

void CServer::startAccept()
{
	auto& io_context = AsioIOServicePool::getInstance().getIOService();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
	_acceptor.async_accept(new_session->getSocket()
		,std::bind(&CServer::handleAccept,this,new_session,std::placeholders::_1));

}


#include <iostream>
#include "cserver.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "asioioservicepool.h"

int main()
{
	try {
		auto& pool = AsioIOServicePool::getInstance();
		boost::asio::io_context io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context, &pool](auto,auto) {
			io_context.stop();
			pool.stop();
		});
		CServer s(io_context, 10086);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}



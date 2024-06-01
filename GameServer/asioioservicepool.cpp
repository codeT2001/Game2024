#include "asioioservicepool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size)
	:_ioServices(size),_works(size),_nextIOService(0)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		_works[i] = WorkPtr(new Work(_ioServices[i]));
	}
	//遍历多个ioservice,创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i)
	{
		_threads.emplace_back([this,i]() {
			_ioServices[i].run();
		});
	}
}
AsioIOServicePool::~AsioIOServicePool()
{
	std::cout << "destruct AsioIOservice Pool" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::getIOService()
{
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size())
	{
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::stop()
{
	//停止io_context
	for (auto& work : _works)
	{
		work.reset();
	}

	//等待子线程退出
	for (auto& t : _threads)
	{
		t.join();
	}
}

AsioIOServicePool& AsioIOServicePool::getInstance()
{
	// TODO: 在此处插入 return 语句
	static AsioIOServicePool instance;
	return instance;
}

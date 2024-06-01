#include "asioioservicepool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size)
	:_ioServices(size),_works(size),_nextIOService(0)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		_works[i] = WorkPtr(new Work(_ioServices[i]));
	}
	//�������ioservice,��������̣߳�ÿ���߳��ڲ�����ioservice
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
	//ֹͣio_context
	for (auto& work : _works)
	{
		work.reset();
	}

	//�ȴ����߳��˳�
	for (auto& t : _threads)
	{
		t.join();
	}
}

AsioIOServicePool& AsioIOServicePool::getInstance()
{
	// TODO: �ڴ˴����� return ���
	static AsioIOServicePool instance;
	return instance;
}

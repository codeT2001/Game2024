#include "logicsystem.h"
LogicSystem::LogicSystem():_b_stop(false)
{
	regCallbacks();
	_worker_therad = std::thread(&LogicSystem::dealMsg,this);
}

LogicSystem::~LogicSystem()
{
	_b_stop = true;
	_cond.notify_one();
	_worker_therad.join();
}

void LogicSystem::postMsgToQue(std::shared_ptr<LogicNode> msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_msg_que.push(msg);
	if (_msg_que.size() == 1)
	{
		_cond.notify_one();
	}

}

LogicSystem& LogicSystem::getInstance()
{
	static LogicSystem instance;
	return instance;
}

void LogicSystem::dealMsg()
{
	for (;;)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		while (_msg_que.empty() && !_b_stop)
		{
			_cond.wait(lock);
		}
		if (_b_stop)
		{
			while (!_msg_que.empty())
			{
				auto msg_node = _msg_que.front();
				std::cout << "recv msg id is " << msg_node->_recvnode->_msg_id << std::endl;
				auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
				if (call_back_iter == _fun_callbacks.end())
				{
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msg_node->_session,msg_node->_recvnode->_msg_id
					,std::string(msg_node->_recvnode->_data,msg_node->_recvnode->_total_len));

				_msg_que.pop();
			}

			break;
		}

		//没有停服并且说明队列中有数据
		auto msg_node = _msg_que.front();
		std::cout << "recv msg id is " << msg_node->_recvnode->_msg_id << std::endl;
		auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
		if (call_back_iter == _fun_callbacks.end())
		{
			_msg_que.pop();
			continue;
		}
		call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id
			, std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));

		_msg_que.pop();
	}
}

void LogicSystem::regCallbacks()
{
	_fun_callbacks[MSG_HW] = std::bind(&LogicSystem::HWCakkback,this
		,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
}
void LogicSystem::HWCakkback(std::shared_ptr<CSession> session
	, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "receive msg id is" << root["id"].asInt() <<
		"msg data is " << root["data"].asString() << std::endl;
	root["data"] = "server has received msg, msg data is" + root["data"].asString();
	
	std::string return_str = root.toStyledString();
	session->send(return_str, root["id"].asInt());

}

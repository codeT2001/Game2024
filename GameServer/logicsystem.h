#pragma once
#include <queue>
#include <thread>
#include "csession.h"
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

typedef std::function<void(std::shared_ptr<CSession> session
	, const short& msg_id, const std::string& msg_data)> funCallback;
class LogicSystem
{
public:
	~LogicSystem();
	void postMsgToQue(std::shared_ptr<LogicNode> msg);
	static LogicSystem& getInstance();
	LogicSystem(const LogicSystem&) = delete;
	LogicSystem& operator = (const LogicSystem&) = delete;
private:
	LogicSystem();
	void dealMsg();
	void regCallbacks();
	void HWCakkback(std::shared_ptr<CSession> session
		, const short& msg_id, const std::string& msg_data);

	std::thread _worker_therad;
	std::queue<std::shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _cond;
	bool _b_stop;
	std::map<short, funCallback> _fun_callbacks;
};


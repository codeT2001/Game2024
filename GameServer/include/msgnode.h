#pragma once
#include <string>
#include "const.h"
#include <iostream>
#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;

class MsgNode
{
public:
	MsgNode(short max_len); 
	~MsgNode();
	void clear();

	short _cur_len;
	short _total_len;
	char* _data;
};

class RecvNode:public MsgNode
{
public:
	RecvNode(short max_len, short msg_id);
	short _msg_id;
};

class SendNode :public MsgNode
{
public:
	SendNode(const char* msg, short max_len, short msg_id);
	short _msg_id;

};

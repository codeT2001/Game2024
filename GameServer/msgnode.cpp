#include "msgnode.h"

MsgNode::MsgNode(short max_len)
	:_total_len(max_len),_cur_len(0)
{
	_data = new char[_total_len+1]();
	_data[_total_len] = '\0';

}

MsgNode::~MsgNode()
{
	std::cout << "destruct MsgNode" << std::endl;
	delete[]_data;
}

void MsgNode::clear()
{
	memset(_data,0,_total_len);
	_cur_len = 0;
}

RecvNode::RecvNode(short max_len, short msg_id)
	:MsgNode(max_len),_msg_id(msg_id)
{

}

SendNode::SendNode(const char* msg, short max_len, short msg_id)
	:MsgNode(max_len+HEAD_TOTAL_LEN),_msg_id(msg_id)
{
	//�ȷ���id��תΪ�����ֽ���
	short msg_id_net = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	memcpy(_data, &msg_id_net, HEAD_ID_LEN);
	//�ٷ������ݣ�תΪ�����ֽ���
	short max_len_net = boost::asio::detail::socket_ops::host_to_network_short(max_len);
	memcpy(_data + HEAD_ID_LEN, &max_len_net, HEAD_DATA_LEN);
	memcpy(_data + HEAD_TOTAL_LEN, msg, max_len);
}

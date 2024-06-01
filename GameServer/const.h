#pragma once
#define MAX_LENGTH 1024*2
#define HEAD_TOTAL_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2

#define MAX_RECVQUE 10000
#define MAX_SENDQUE 1000


enum MSG_IDS {
	MSG_HW = 1001,
	MSG_LOGIN = 1002,
};

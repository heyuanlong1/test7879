#ifndef __BASE__
#define __BASE__


#define LOGIN  	1001
#define READY  	1002
#define MSG  	1003
#define LEAVE  	1004


typedef struct head_s head_t;
struct head_s
{
	int size;
	int cmd;
};

typedef struct login_s login_t;
struct login_s
{
	head_t head;
	int userid;
	char token[32];
};

typedef struct ready_s ready_t;
struct ready_s
{
	head_t head;
	int userid;
};

typedef struct msg_s msg_t;
struct msg_s
{
	head_t head;
	int userid;
	int dataSize;
	char data[0];
};

typedef struct leave_s leave_t;
struct leave_s
{
	head_t head;
	int userid;
	int score;
};



#endif
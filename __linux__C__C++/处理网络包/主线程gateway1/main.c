#include "stdio.h"
#include "gateway.h"
#include "base.h"

void test_open()
{
	printf("test_open\n");
}
void test_connect(const int fd,const char * addr)
{
	printf("test_connect fd:%d,addr:%s\n",fd, addr);
}
void test_disconnect (const int fd)
{
	printf("test_disconnect fd:%d\n", fd);
}
void test_error(const int fd,const char * msg)
{
	printf("test_error fd:%d,msg:%s\n",fd, msg);
}
void test_message(const int fd,const void * msg,const int size)
{

	msg_t *que = (msg_t*)(msg);
	printf(" fd:%d,MSG:useid:%d,size:%d",fd,que->userid,size);
	write(STDOUT_FILENO,que->data,que->dataSize);
	printf("\n");

}

int main(int argc, char const *argv[])
{
	
	struct gateway_handle handle = {
		.open = test_open,
		.connect = test_connect,
		.disconnect = test_disconnect,
		.error = test_error,
		.message = test_message,
	};
	set_config("0.0.0.0",6001,10000,true);
	start(handle);

	return 0;
}
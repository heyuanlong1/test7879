#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <sys/time.h>
#include <unistd.h>

#include "MsThreadBase.h"


class testProcess : public MsThreadBase
{
public:
	testProcess(){}
	~testProcess(){}

	void process(){
		while(1){
			usleep(1 * 1000000);
			printf("process\n");
		}
	}
};

int main(int argc, char const *argv[])
{
	testProcess p;
	p.startThread();

	getchar();
	return 0;
}
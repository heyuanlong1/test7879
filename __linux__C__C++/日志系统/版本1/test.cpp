#include <time.h>
#include <stdio.h>
#include "log.h"

int main()
{
	
	SET_LOG_NAME("./test.log");
	SET_LOG_LEVEL(LOG_LEVEL_TRACE);
	SET_LOG_SIZE(1000 * 1024);
	
	
	clock_t start, finish;
    double duration;
    start = clock();

    for(int i = 0; i < 100000; i++)
    {
        LOG_TRACE("****%d****", i);
        LOG_INFO("test INFO");
        LOG_WARNING("test WARNING");
        LOG_ERROR("test ERROR");
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("duration = %.3f.\n", duration);
	
	return 0;
}


#include <time.h>
#include <stdio.h>
#include "log.h"

int main()
{
	

	
	ko_log_init();
	ko_log_set_path( "./" );
	ko_log_set_level(KO_LOG_LEVEL_MAX);
	
	
	clock_t start, finish;
    double duration;
    start = clock();

    for(int i = 0; i < 1000; i++)
    {        ko_log_warn("test WARNING");
        ko_log_error("test ERROR");
        ko_log_sys_debug("****%d****", i);
        ko_log_sys_info("test INFO");

    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("duration = %.3f.\n", duration);
	
	ko_log_set_path_for_printf();
	
	for(int i = 0; i < 10; i++)
    {       
		ko_log_warn("test WARNHNG");
        ko_log_error("test ERROR");
        ko_log_sys_debug("****%d****", i);
        ko_log_sys_info("test INFO");
    }

	
	return 0;
}


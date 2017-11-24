#include "vs_mem.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char const *argv[])
{
	time_t timep;
	int i,j,status;
	int tempNums=0;
	
	ngx_pool_t * pool = ngx_create_pool(1024);
	
	time(&timep);
	printf("time(): %d\n",timep);
	
	for ( j = 0; j < 10; ++j)
	{
		ngx_palloc(pool,800);
	}
	for ( j = 0; j < 10; ++j)
	{
		ngx_palloc(pool,2000);
	}
	time(&timep);
	printf("time(): %d\n",timep);
	printf("block: %d\n",pool->block_nums);
	printf("large: %d\n",pool->large_nums);
	
	
	ngx_reset_pool(pool);
	printf("time(): %d\n",timep);
	printf("block: %d\n",pool->block_nums);
	printf("large: %d\n",pool->large_nums);
	
	ngx_destroy_pool(pool);
	return 0;
}
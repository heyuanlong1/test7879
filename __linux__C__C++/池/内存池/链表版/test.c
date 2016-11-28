#include "hMemoryPool.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char const *argv[])
{
	initStruct preSizeArr[] = {{40,2000},{60,400},{88,300},{120,200}};
	int preNums = sizeof(preSizeArr) / sizeof(initStruct);
	int i,j,status;
	char buf[100];
	void *temp;
	clock_t start, finish; 
	int dur;
	time_t timep;



	initHMemoryPool(preSizeArr,preNums);

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);

	void *tempArr[26000];
	int tempNums=0;
	for ( i = 0; i < 26000; ++i)
	{
		tempArr[i] = getHMemory(19,&status);
		tempNums = i + 1;
	}

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);

	for ( i = 0; i < tempNums; ++i)
	{
		freeHMemory(tempArr[i]);
	}
/*	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);
	
	setHMemoryNums(120,10000);
	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);
	
	setHMemoryNums(100,5000);
	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);
	
	for ( i = 0; i < 100; ++i)
	{
		setHMemoryNums(i,100);
	}*/

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	//以下测试要求机子的内存够大
	setHMemoryNums(20,10000);
	time(&timep);
	printf("time(): %d\n",timep);
	for ( j = 0; j < 9000; ++j)
	{
		for ( i = 0; i < 10000; ++i)
		{
			tempArr[i] = getHMemory(20,&status);
			tempNums = i + 1;
		}
		for ( i = 0; i < tempNums; ++i)
		{
			freeHMemory(tempArr[i]);
		}
	}

	time(&timep);
	printf("time(): %d\n",timep);

	for ( j = 0; j < 9000; ++j)
		{
		for ( i = 0; i < 10000; ++i)
		{
			tempArr[i] = malloc(20);
			tempNums = i + 1;
		}
		for ( i = 0; i < tempNums; ++i)
		{
			free(tempArr[i]);
		}
	}
	time(&timep);
	printf("time(): %d\n",timep);

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	read(STDIN_FILENO,buf,sizeof(buf));
	return 0;
}
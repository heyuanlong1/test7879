#ifndef HMEMORY_POOL_H_
#define HMEMORY_POOL_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blockHead blockHead;
struct blockHead
{
	int size;
	char inPool;
	void *next;
};


typedef struct slot slot;
struct slot
{
	void *pFree;
	int blockSize;
	int freeNums;
	int allNums;
	int maxNums;
};

typedef struct initStruct initStruct;
struct initStruct
{
	int size;
	int nums;
};

//初始化内存，内存情况，用户自定义
int initHMemoryPool(initStruct* arr,int nums);

//获取大小为size的内存，
//检查status，-2  为size过大，返回的非内存池里的空间，
//			  -1  此size的block，全部分配出去咯，返回的非内存池里的空间，
//            MALLOCFAIL, 失败了。
void* getHMemory(int size,int *status);

//释放内存
int freeHMemory(void *p);

//设置此size大小的block的最大数量。
int setHMemoryNums(int size,int maxNums);

//打印内存池当前状态。
int printHMemory(int fd);


#endif
#include "hMemoryPool.h"


#define PRENUMS 100
#define DOUBLENUMS 5
#define BIGGERNUMS 2
#define MALLOCFAIL -9

static slot *slotArr = NULL;
static int slotArrCurrentNums = 0;
static int slotArrAllNums = 0;
static initStruct preSizeArr[] = {{20,5000},{30,5000},{40,5000},{50,5000}} ;


//内部使用的函数
int quickSort(slot *R,int s,int t);					//快速排序
int removeRepeat(slot *R,int *nums);				//去重以及去掉blockSize<=0的
int Search(slot *R,int nums,int size);				//查找size的索引位置
int biggerMalloc(slot *sr,int nums,char add);		//给单个slot分配block
int resetSlotArr();									//重置slot数组

int initHMemoryPool(initStruct* arr,int nums)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int preNums = 0;
	int returnRes;

	preNums = sizeof(preSizeArr) / sizeof(initStruct);
	slotArrAllNums = nums + preNums + PRENUMS ;
	slotArr = (slot*)malloc(sizeof(slot) * slotArrAllNums);
	if (slotArr == NULL){
		return MALLOCFAIL;
	}
	memset(slotArr,0,sizeof(slot) * slotArrAllNums);

//用户预分配的
	for ( i = 0; i < nums; ++i)
	{
		slotArr[i].blockSize = arr[i].size;
		slotArr[k].pFree = NULL;
		slotArr[i].freeNums = arr[i].nums;
		slotArr[i].allNums = arr[i].nums;
		slotArr[i].maxNums = arr[i].nums * DOUBLENUMS;
	}
	slotArrCurrentNums = nums;
	
//系统预分配的,如果用户分配了，就不加上去了。
	k = slotArrCurrentNums;
	for ( i = 0; i < preNums; ++i)
	{
		for ( j = 0; j < slotArrCurrentNums; ++j)
		{
			if (preSizeArr[i].size == slotArr[j].blockSize){
				break;				
			}
		}
		if (j == slotArrCurrentNums){
			slotArr[k].blockSize = preSizeArr[i].size;
			slotArr[k].pFree = NULL;
			slotArr[k].freeNums = preSizeArr[i].nums;
			slotArr[k].allNums = preSizeArr[i].nums;
			slotArr[k].maxNums = preSizeArr[i].nums * DOUBLENUMS;
			++k;
		}
	}
	slotArrCurrentNums = k;


//排序----------
	quickSort(slotArr,0, slotArrCurrentNums-1 ); 

//去重以及去掉blockSize<=0的------
	if( (returnRes = removeRepeat(slotArr,&slotArrCurrentNums)) != 0 ){
		return returnRes;
	}

//分配slot数组的block
	for (i = 0; i < slotArrCurrentNums; ++i)
	{
		if(slotArr[i].allNums <= 0 ){
			continue;
		}else{			
			if(biggerMalloc(slotArr + i,slotArr[i].allNums,'0') == MALLOCFAIL){
				return MALLOCFAIL;
			} 
		}
	}

	return 0;
}

void* getHMemory(int size,int *status)
{
	int searchRes;
	void *res = NULL;
	blockHead *temp = NULL;
	int mNums;
	int headSize = sizeof(blockHead);

	searchRes = Search(slotArr,slotArrCurrentNums,size);
	if(searchRes == slotArrCurrentNums){				//size过大
		*status = -2;
		temp =(blockHead *)malloc(headSize + size);
		temp->size = size;
		temp->inPool = '0';								//非池中之物
		res = (void*)(temp) + headSize;
	}else{												
		if (slotArr[searchRes].pFree == NULL){									//没有空余的block了，试图分配
			if (slotArr[searchRes].maxNums > slotArr[searchRes].allNums){		//可以继续分配block
				mNums = slotArr[searchRes].allNums * BIGGERNUMS;
				if ( (mNums + slotArr[searchRes].allNums) > slotArr[searchRes].maxNums){
					mNums = slotArr[searchRes].maxNums - slotArr[searchRes].allNums;
				}
				if(biggerMalloc(slotArr + searchRes,mNums,'1') == MALLOCFAIL){		
					*status = MALLOCFAIL;
					return NULL;
				} 
			}			
		}

		if (slotArr[searchRes].pFree == NULL){			//没有空余的block了
			*status = -1;
			temp =(blockHead *)malloc(headSize + size);
			temp->size = size;
			temp->inPool = '0';							//非池中之物
			res = (void*)(temp) + headSize;
		}else{											//还有空余的block
			*status = 0;
			res = slotArr[searchRes].pFree;

			temp = (blockHead *)(res - headSize);
			slotArr[searchRes].pFree = temp->next;
			--slotArr[searchRes].freeNums;
		}	
	}
	return res;
}

int freeHMemory(void *p)
{
	int res;
	int searchRes;
	int headSize = sizeof(blockHead);
	blockHead *temp = (blockHead *)(p - headSize);		//
	int size = temp->size;
	char tempInPool = temp->inPool;
	if (tempInPool == '0'){		//不在池里的
		free(p-headSize);
	}else{
		searchRes = Search(slotArr,slotArrCurrentNums,size);
		if(searchRes == slotArrCurrentNums){
								//理论上不会执行到这里，因为过大的size不会在池里
		}else{
			temp->next = slotArr[searchRes].pFree;
			slotArr[searchRes].pFree = p;
			++slotArr[searchRes].freeNums;
		}
	}
}

int resetSlotArr()
{
	int tempAllNums;
	slot *tempSlotArr;
	int i;

	tempAllNums = slotArrCurrentNums * 2 ;	//slot数组变为原来的2倍
	tempSlotArr = (slot*)malloc(sizeof(slot) * tempAllNums);
	if (tempSlotArr == NULL){
		return MALLOCFAIL;
	}
	for (i = 0; i < slotArrCurrentNums; ++i)
	{
		tempSlotArr[i] = slotArr[i];
	}
	slotArr = tempSlotArr;
	slotArrAllNums = tempAllNums;
	return 0;
}

int setHMemoryNums(int size,int maxNums)
{
	int i;
	int j;
	int tempRes;
	slot temp;

	if(size <=0 || maxNums <= 0){
		return 0;
	}

	if(slotArrCurrentNums == slotArrAllNums){		//slot数组不够长了，加长。
		if( (tempRes = resetSlotArr()) != 0){
			return tempRes;
		}
	}

	for ( i = 0; i < slotArrCurrentNums; ++i)		//找到此size的在哪个索引之间
	{
		if(slotArr[i].blockSize >= size ){
			break;
		}
	}
	if(i < slotArrCurrentNums){						//小于最大的blockSize，则测试有没有同样大小的size
		if (slotArr[i].blockSize == size){ 
			if (maxNums > slotArr[i].maxNums){
				slotArr[i].maxNums = maxNums;				
			}
			return 0;
		}
	}

	temp.pFree = NULL;								//给新的blockSize分配slot和block
	temp.blockSize = size;
	temp.freeNums = 0;
	temp.allNums = 0;
	temp.maxNums = maxNums * DOUBLENUMS;	
	if(biggerMalloc(&temp,maxNums,'1') == MALLOCFAIL){
		return -1;
	}

	for ( j = slotArrCurrentNums-1 ; j >= i ; --j)	//把新的slot加入到slot数组里
	{
		slotArr[ j+1 ] = slotArr[ j ];
	}
	slotArr[i] = temp;
	++slotArrCurrentNums;							//当然别忘记了slotArrCurrentNums加一

	return 0;
}


int printHMemory(int fd)
{
	int i;
	const int bufSize = 512;
	char buf[bufSize];
	for ( i = 0; i < slotArrCurrentNums; ++i)
	{
		sprintf(buf,"blockSize:%10d  freeNums:%10d  allNums:%10d  maxNums:%10d\n",slotArr[i].blockSize,slotArr[i].freeNums,slotArr[i].allNums,slotArr[i].maxNums);
		write(fd,buf,strlen(buf));
	}
	return 0;
}



int quickSort(slot *R,int s,int t)  
{  
    int i=s,j=t;  
    slot tmp;  
    if (s<t)                 //区间内至少存在两个元素的情况  
    {  
        tmp=R[s];           //用区间的第1个元素作为基准  
        while (i!=j)        //从区间两端交替向中间扫描,直至i=j为止  
        {  
            while (j>i && R[j].blockSize  >= tmp.blockSize)   
                j--;        //从右向左扫描,找第1个小于tmp的R[j]  
            R[i]=R[j];      //找到这样的R[j],R[i]和R[j]交换  
            while (i<j && R[i].blockSize <= tmp.blockSize)   
                i++;        //从左向右扫描,找第1个大于tmp的元素R[i]  
            R[j]=R[i];      //找到这样的R[i],R[i]和R[j]交换  
        }  
        R[i]=tmp;  
        quickSort(R,s,i-1); //对左区间递归排序  
        quickSort(R,i+1,t); //对右区间递归排序  
    }  
    return 0;
}  

int removeRepeat(slot *R,int *nums)
{
	int i,pre,next;
	char *temp;
	int tempNums;
	if(*nums < 2){
		return 0;
	}
	temp = (char*)malloc( sizeof(char) * (*nums) );
	if (temp == NULL){
		return MALLOCFAIL;
	}
	tempNums = *nums;
	pre = 0;
	next = 1;

	temp[pre]= '0';							//'0'为要去掉的
	if (R[pre].blockSize <=0 ){
		temp[pre] = '1';
	}
	for (; next < *nums; ++pre, ++next)
	{
		if(R[pre].blockSize == R[next].blockSize || R[next].blockSize <= 0){  //去重以及去掉blockSize<=0的
			temp[next] = '1';
		}else{
			temp[next] = '0';
		}
	}

	*nums = 0;
	for ( i = 0; i < tempNums; ++i)
	{
		if (temp[i] == '0'){
			R[*nums] = R[i];
			++(*nums);
		}
	}
	return 0;
}



int Search(slot *R,int nums,int size)
{
	int i;
	for ( i = 0; i < nums; ++i)
	{
		if ( size <= R[i].blockSize){
			return i;
		}
	}
	return nums;
}

int biggerMalloc(slot *sr,int nums,char add)
{	
	int headSize,justBlockSize,allBlockSize;
	void *head,*temp,*slotpFree;
	blockHead *headTemp;
	int j;

	headSize = sizeof(blockHead);
	justBlockSize = sr->blockSize;
	allBlockSize = headSize + justBlockSize;

	head = malloc( allBlockSize * nums );
	if(head == NULL){
		return MALLOCFAIL;
	}
	slotpFree = sr->pFree;
	sr->pFree = head + headSize;		//加入到block链表的头部
	if (add == '1'){					//是否为新增，而非初始化
		sr->freeNums += nums;
		sr->allNums += nums;
	}
	temp = head;
	for ( j = 0; j < nums; ++j)
	{
		headTemp = (blockHead*)temp;
		headTemp->size = sr->blockSize;
		headTemp->inPool = '1';			//池中之物
		if (j == (nums - 1) ){					
			headTemp->next = slotpFree;
		}else{
			headTemp->next = temp + allBlockSize + headSize;
		}
		temp += allBlockSize;
	}

	return 0;
}
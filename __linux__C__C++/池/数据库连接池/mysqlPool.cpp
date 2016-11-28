#include "mysqlPool.h"

mysqlPool* mysqlPool::mInstance		= NULL;
int mysqlPool::nums					= 4;
std::string mysqlPool::smHost		= "localhost";
std::string mysqlPool::smUser		= "root";
std::string mysqlPool::smPassword	= "4hylWcHNJGp";
std::string mysqlPool::smDb			= "test";

mysqlPool::~mysqlPool()
{
	std::list<MYSQL *>::iterator b,e;
	mLock.lock();
	b = mOutList.begin();
	e = mOutList.end();
	for (; b != e; ++b)
	{
		delete (*b);
	}
	
	b = mInList.begin();
	e = mInList.end();
	for (; b != e; ++b)
	{
		delete (*b);
	}
	mLock.unlock();

}

mysqlPool* mysqlPool::getInstance()
{
	if (mInstance == NULL)
	{
		static mysqlPool sObj;
		mysqlPool::mInstance = &sObj; 
	}
	return mysqlPool::mInstance;
}

mysqlPool::mysqlPool()
{
	for (int i = 0; i < mysqlPool::nums; ++i)
	{
		MYSQL *temp = (MYSQL*)malloc(sizeof(MYSQL));
		if (temp == NULL){
			printf("malloc failed\n");
			exit(0);
		}
		if(mysql_init(temp) == NULL){
			printf("mysql_init failed\n");
			exit(0);
		}
		if (mysql_real_connect(temp,mysqlPool::smHost.c_str(),mysqlPool::smUser.c_str(),mysqlPool::smPassword.c_str(),mysqlPool::smDb.c_str(),0,NULL,0) == NULL){
			printf("mysql_real_connect failed\n");
			exit(0);
		}
		if(mysql_set_character_set(temp,"utf8") != 0){
			printf("mysql_set_character_set failed\n");
			exit(0);
		}
		mInList.push_front(temp);

	}
}

MYSQL* mysqlPool::getConn()
{
	MYSQL *temp = NULL;
	mLock.lock();
	if (mInList.size() == 0){
		temp = NULL;
	}else{
		temp = mInList.front();
		mInList.pop_front();	
	}
	mLock.unlock();
	if(temp != NULL){
		if(mysql_ping(temp)){
			if(mysql_init(temp) == NULL){
				perror("mysql_init failed\n");
				return NULL;
			}
			if (mysql_real_connect(temp,mysqlPool::smHost.c_str(),mysqlPool::smUser.c_str(),mysqlPool::smPassword.c_str(),mysqlPool::smDb.c_str(),0,NULL,0) == NULL){
				perror("mysql_real_connect failed\n");
				return NULL;
			}
			if(mysql_set_character_set(temp,"utf8") != 0){
				perror("mysql_set_character_set failed\n");
				return NULL;
			}
		}
		mLock.lock();
		mOutList.push_front(temp);
		mLock.unlock();
	}
	return temp;
}

MYSQL* mysqlPool::recycConn(MYSQL *temp)
{

	std::list<MYSQL *>::iterator b,e;
	mLock.lock();
	b = mOutList.begin();
	e = mOutList.end();
	for (; b != e; ++b)
	{
		if( *b == temp){
			break;
		}
	}
	if(b == e){
		mLock.unlock();
		return NULL;
	}

	b = mInList.begin();
	e = mInList.end();
	for (; b != e; ++b)
	{
		if( *b == temp){
			break;
		}
	}
	if(b != e){
		mLock.unlock();
		return NULL;
	}

	mOutList.remove(temp);
	mInList.push_front(temp);
	mLock.unlock();
	return temp;
}


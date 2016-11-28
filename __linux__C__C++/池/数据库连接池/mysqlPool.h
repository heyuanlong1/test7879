#ifndef __MYSQLPOOL__H_
#define __MYSQLPOOL__H_

#include "mutex.h"
#include "mysql.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <list>
class mysqlPool
{
public:
	~mysqlPool();

	static mysqlPool* getInstance();
	MYSQL* getConn();
	MYSQL* recycConn(MYSQL *temp);

private:
	mysqlPool();

	CommonMutex mLock;
	std::list<MYSQL*> mInList;
	std::list<MYSQL*> mOutList;

	static mysqlPool* mInstance;
	static int nums;
	static std::string smHost;
	static std::string smUser;
	static std::string smPassword;
	static std::string smDb;
	/* data */
};
#endif
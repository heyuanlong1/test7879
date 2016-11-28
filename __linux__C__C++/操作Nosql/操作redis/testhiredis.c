#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

int main(int argc, char const *argv[])
{
	unsigned int j;
	redisContext *c;
	redisReply *reply;

	const char *hostname  = (argc > 1 ) ? argv[1] : "127.0.0.1";
	int port = (argc >2 ) ? atoi(argv[2]) :6379;

	struct timeval timeout = {1,500000}; //1.5second
	c = redisConnectWithTimeout(hostname,port,timeout);
	if ( c== NULL || c->err){
		if(c){
			printf("Connect error :%s\n",c->errstr );
			redisFree(c);
		}else{
			printf("Connection error: can't allocate redis context\n");
		}
		exit(1);
	}

	reply = redisCommand(c,"auth ******");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}

	reply = redisCommand(c,"ping");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("ping:%s\n", reply->str);
	freeReplyObject(reply);

	reply = redisCommand(c,"set %s %s","foo","hello world");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("set :%s\n", reply->str);
	freeReplyObject(reply);

	/* Set a key using binary safe API */
	reply = redisCommand(c,"set %b %b","bar",(size_t)3,"hello",(size_t)5);
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("set (binary api)%s\n", reply->str);
	freeReplyObject(reply);

	reply = redisCommand(c,"get foo");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("get foo:%s\n",reply->str );
	freeReplyObject(reply);

	reply = redisCommand(c,"incr counter");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("incr counter %lld\n",reply->integer );
	freeReplyObject(reply);

	reply = redisCommand(c,"incr counter");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("incr counter %lld\n",reply->integer );
	freeReplyObject(reply);

	reply = redisCommand(c,"hmset field field1 1 field2 2");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	printf("hmset:  %s\n",reply->str );
	freeReplyObject(reply);

	reply = redisCommand(c,"hmget field field1 field2 ");
	if (reply->type == REDIS_REPLY_ERROR){
		printf("Error: %s\n", reply->str);
	}
	if (reply->type != REDIS_REPLY_ARRAY){
		printf("Unexpected type: %d\n", reply->type);
	}else{
		if(reply->elements != 2){
			printf("Unexpected nums\n");
		}else{
			for (j = 0; j < reply->elements; ++j)
			{
				printf("%u, %s\n",j,reply->element[j]->str );
			}
		}
	}
	freeReplyObject(reply);

	redisFree(c);
	return 0;
}

/*
ping:PONG
set :OK
set (binary api)OK
get foo:hello world
incr counter 11
incr counter 12
hmset:  OK
0, 1
1, 2


*/
#!/usr/bin/python
# -*- coding:UTF-8 -*-


import redis

REDIS_HOST		='localhost'
REDIS_PORT		= 6379
REDIS_AUTH 		= '*********'

class RedisDB():
	def __init__(self):
		self.redis_connection = None

	def __del__(self):
		pass
	def init(self):
		try:
			self.redis_connection = redis.Redis(host=REDIS_HOST,port=REDIS_PORT,db=0,password=REDIS_AUTH);
		except Exception, e:
			print "init redis connect fail"
			raise e
	def reConnect(self):
		try:
			self.redis_connection.close()
		except Exception, e:
			pass

		try:
			self.redis_connection = redis.Redis(host=REDIS_HOST,port=REDIS_PORT,db=0,password=REDIS_AUTH);
		except Exception, e:
			print "redis reConnect:",e
			raise e
	def getConnect(self):
		return self.redis_connection

	def set(self,key,value):
		try:
			return self.redis_connection.set(key,value)
		except Exception, e:
			print "redis error:",e

	def get(self,key):
		try:
			return self.redis_connection.get(key)
		except Exception, e:
			print "redis error:",e
	def hset(self,key,field,value):
		try:
			return self.redis_connection.hset(key,field,value)
		except Exception, e:
			print "redis error:",e	

	def hget(self,key,field):
		try:
			return self.redis_connection.hget(key,field)
		except Exception, e:
			print "redis error:",e	

	def hmset(self,key,table):
		try:
			return self.redis_connection.hmset(key,table)
		except Exception, e:
			print "redis error:",e	

	def hmget(self,key,arr):
		try:
			return self.redis_connection.hmget(key,arr)
		except Exception, e:
			print "redis error:",e	



# r = RedisDB();
# r.init()

# print r.set("test","1111")
# print r.get("test")
# print ""

# print r.hset("keykey","field1",12)
# print r.hget("keykey","field1")
# print ""

# print r.hmset("keykey",{'field1':1,'field2':2,'field3':3})
# print r.hmget("keykey",["field1","field2","field3"])
# print ""


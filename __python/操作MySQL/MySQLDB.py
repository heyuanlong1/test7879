#!/usr/bin/python
# -*- coding:UTF-8 -*-

#数据库封装类

import MySQLdb
import time, os, sched 

##全局定义
MYSQL_HOST		='localhost'
MYSQL_PORT		=3306
MYSQL_USER		='root'
MYSQL_PASSWORD	='*********'



class RankDB:
	def __init__(self):
		self.sql_connect = None
		self.cursor = None
		self.initSql()

	def __del__(self):
		try:
			self.cursor.close()
		except Exception, e:
			pass
		try:
			self.sql_connect.close()
		except Exception, e:
			pass

	def initSql(self):
		try:
			self.sql_connect=MySQLdb.connect(host=MYSQL_HOST,port=MYSQL_PORT,user=MYSQL_USER,passwd=MYSQL_PASSWORD) 
			self.cursor = self.sql_connect.cursor() 
		except Exception,e:
			print "initSql fail:",Exception,":",e, "time:",time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
			raise e

	def reConnect(self):
		try:
			self.cursor.close()
		except Exception, e:
			pass
		try:
			self.sql_connect.close()
		except Exception, e:
			pass

		try:
			self.sql_connect=MySQLdb.connect(host=MYSQL_HOST,port=MYSQL_PORT,user=MYSQL_USER,passwd=MYSQL_PASSWORD) 
			self.cursor = self.sql_connect.cursor() 
		except Exception, e:
			print "reconnect fail:", Exception,":",e, "time:",time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
			raise e

	def excuteSql(self,sql_str):
	    try:
	    	self.cursor.execute(sql_str)
	    except Exception, e:
	    	x,y = e.args
	    	if x == 1064:
	    		print y,":",sql_str
	    	else:
		    	print Exception,":",e, time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time())) 
		        print "reconnect"
		        self.reConnect()
		        self.cursor.execute(sql_str)
	        	        	

	def fetchall(self):
		return self.cursor.fetchall()
		
	def rowcount(self):
		return self.cursor.rowcount()
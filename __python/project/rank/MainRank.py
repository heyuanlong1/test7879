#!/usr/bin/python
# -*- coding:UTF-8 -*-

"""
设计流程图 ：https://www.processon.com/view/link/57e0cabce4b0a16a66f508f2
数据库相关表：
	ko_rank_config.rank_config
	ko_rank.battle_item_rank_history
	ko_rank.battle_item_rank_history_YYYY_MM
	ko_rank.battle_item_rank_history_YYYY_MM_DD
	ko_rank.battle_item_rank_history_YYYY_MM_WW


关于这里的定时器：
这里定时为2秒，如果排行超过2秒，就会直接走下一个排行，如果时间不超过2秒，那么会等到满足2秒才会走下一个排行。
"""

from RankConfig import RankConfig
from Rank import Rank
from RankDB import RankDB 
import time, os, sched ,math

schedule = sched.scheduler(time.time, time.sleep) 	#定时执行work

class MainRank():
	def __init__(self):			
		self.m_inc_second = 2			#每m_inc_second秒刷新一次
		self.m_reset_max_time = 1800 / self.m_inc_second	#每m_reset_max_time秒刷新一次配置
		self.m_reset_time=0
		self.m_rank_config = None
		self.m_db = None
		self.m_config_dict = None
		self.m_last_time_beg = 0
		self.m_time_beg = 0
		self.m_end_beg = 0

	def initLoad(self):
		self.m_rank_config = RankConfig()
		self.m_db = RankDB()
		self.m_config_dict = self.m_rank_config.getDict()


	def getConfig(self):
		keys_list = self.m_rank_config.getKeys()
		sql_str = "select rankID,gameID,\
					type,tableName,factorA, \
					ruleA,whereA,sortLimit,status \
					from ko_rank_config.rank_config"
		self.m_db.excuteSql(sql_str)
		results = self.m_db.fetchall()			
		for row in results:
			if keys_list.__contains__(row[0]) == False:		#新的排行
				temp_rank = Rank()
				temp_rank.initLoad(row[0],row[1],   row[2],row[3],row[4],   row[5],row[6],row[7],row[8])
				temp_rank.initData()
				self.m_rank_config.set(row[0],temp_rank)
			else:
				keys_list.remove(row[0])	#本来就有的从keys_list里删掉
				temp_rank_exist = self.m_config_dict.get( row[0] )
				if temp_rank_exist.haveChange(row[1],   row[2],row[3],row[4],   row[5],row[6],row[7],row[8]) == False:
					temp_rank_exist.initLoad(row[0],row[1],   row[2],row[3],row[4],   row[5],row[6],row[7],row[8])
					temp_rank_exist.initData()
				else:
					pass

		for x in keys_list:				#del被删除的排行
			self.m_rank_config.delKey(x)
			#print "self.m_rank_config.delKey",x

		self.m_config_dict = self.m_rank_config.getDict()


	def performCommand(self): 
		schedule.enter(m.m_inc_second, 0, self.performCommand, () )
		self.m_last_time_beg = self.m_time_beg
		m_last_time_beg_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(self.m_last_time_beg))
		self.m_time_beg = time.time()
		m_time_beg_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(self.m_time_beg))

		self.m_reset_time += 1
		if self.m_reset_time >= self.m_reset_max_time :
			self.getConfig()
			self.m_reset_time = 0
		else:
			pass

		for rank_id in self.m_config_dict:
			temp_rank = self.m_config_dict.get(rank_id)
			temp_rank.loop(self.m_time_beg,self.m_inc_second)

		self.m_time_end = time.time()
		consume_time  = self.m_time_end - self.m_time_beg
		print "timing:",m.m_inc_second,"  consume time:",consume_time,"\n"

		if consume_time > m.m_inc_second:
			m.m_inc_second = math.ceil(consume_time)		#定时时间变长			
			if consume_time > 5:							#大于一定时间应该报警了
				print "-----------------------waring\n"

	def timmingExe(self ): 
	    schedule.enter(m.m_inc_second, 0, self.performCommand, () ) 
	    schedule.run() 

m = MainRank()
m.initLoad()
m.getConfig()
m.timmingExe()



#!/usr/bin/python
# -*- coding:UTF-8 -*-

"""


"""
import time,datetime
from RankDB import RankDB 
from RankRedis import RankRedis 

class Rank :
	def __init__(self):
		#数据库属性
		self.m_db_rank_id 	= -1
		self.m_db_game_id 	= -1
		self.m_db_type 	 	= -1
		self.m_db_db_name 	= ""
		self.m_db_factor_a 	= ""
		self.m_db_rule_a	= ""
		self.m_db_where_a	= ""
		self.m_db_sort_limit = 0
		self.m_db_status	= 0

		self.m_insert_table	= ""

		#辅助属性
		self.m_min_value	= -9999999
		self.m_timestamp	= 0
		self.m_need_sort	= False
		self.m_db			= None
		self.m_redis		= None
		self.m_unsort_dic 	= {}
		self.m_sort_dic 	= {}
		self.str_type_limit_time = ""

		#凌晨操作时间
		self.m_hour			= 0
		self.m_min			= 0
		self.m_sec			= 0 	#最好小于 10，见dawnOperate

	def __del__(self):
		del self.m_db
		self.m_unsort_dic.clear()
		self.m_sort_dic.clear()
	def haveChange(self,game_id,tmp_type,db_name,factor_a,rule_a,where_a,sort_limit,status):
		if self.m_db_game_id 	!= game_id :
			return False
		if self.m_db_type 	 	!= tmp_type :
			return False
		if self.m_db_db_name 	!= db_name :
			return False
		if self.m_db_factor_a 	!= factor_a :
			return False
		if self.m_db_rule_a		!= rule_a :
			return False
		if self.m_db_where_a	!= where_a :
			return False
		if self.m_db_sort_limit != sort_limit :
			return False
		if self.m_db_status		!= status :
			return False

		return True

	def initLoad(self,rank_id,game_id,tmp_type,db_name,factor_a,rule_a,where_a,sort_limit,status): 	
		self.m_db_rank_id 	= rank_id
		self.m_db_game_id 	= game_id
		self.m_db_type 	 	= tmp_type
		self.m_db_db_name 	= db_name
		self.m_db_factor_a 	= factor_a

		if rule_a == "asc":
			self.m_db_rule_a = "asc"
		else:
			self.m_db_rule_a = "desc"
			
		self.m_db_where_a	= where_a
		self.m_db_sort_limit = sort_limit
		self.m_db_status		= status

		if self.m_db_status != 1:
			return

		self.m_min_value	= -9999999
		self.m_need_sort	= False
		del self.m_db
		self.m_db 			= RankDB()
		self.m_redis 		= RankRedis()

	def initData(self):	
		if self.m_db_status != 1:
			return

		self.m_timestamp 	= time.time()
		#根据type 确定 where editTime
		self.str_type_limit_time = ""
		if self.m_db_type == 0:			#每日
			self.dawnOperateType_0(self.m_timestamp)
			self.str_type_limit_time = time.strftime("%Y-%m-%d", time.localtime(self.m_timestamp))

		elif self.m_db_type == 1:		#历史
			self.m_insert_table = "ko_rank.battle_item_rank_history"
			self.str_type_limit_time = ""
		elif self.m_db_type == 2:		#周
			self.dawnOperateType_2(self.m_timestamp)
			tl = time.localtime(self.m_timestamp)
			self.str_type_limit_time = time.strftime("%Y-%m-%d", time.localtime(self.m_timestamp - tl.tm_wday * 3600 * 24))
		elif self.m_db_type == 3:		#月
			self.dawnOperateType_3(self.m_timestamp)
			self.str_type_limit_time = time.strftime("%Y-%m", time.localtime(self.m_timestamp))
		else:
			return

		sql_str = "select userID," + self.m_db_factor_a + " from " + self.m_db_db_name + "\
				 where 1=1 and gameID ="+ str(self.m_db_game_id) +" and "+ self.m_db_where_a + " and editTime>='" + self.str_type_limit_time + "' \
				 order by "+ self.m_db_factor_a + " " + self.m_db_rule_a + " \
				 limit " + str(self.m_db_sort_limit)

		print "rank_id:",self.m_db_rank_id,"--",sql_str
		self.m_db.excuteSql(sql_str)
		results = self.m_db.fetchall()			
		for row in results:
			self.m_unsort_dic[ row[0] ] = row[1]

		self.sortData()

	def dawnOperate(self,timestamp,last_second):
		temp_tl = time.localtime(timestamp)
		if self.m_db_type == 0:			#每日
			if temp_tl.tm_hour == self.m_hour and temp_tl.tm_min == self.m_min and temp_tl.tm_sec >= self.m_sec and  temp_tl.tm_sec <= (self.m_sec + last_second -1):
				self.dawnOperateType_0(timestamp)
				print "------------------dawnOperate -> dawnOperateType_0"
			else:
				pass

		elif self.m_db_type == 1:		#历史
			pass

		elif self.m_db_type == 2:		#周		
			if temp_tl.tm_wday == 0 and temp_tl.tm_hour == self.m_hour and temp_tl.tm_min == self.m_min and temp_tl.tm_sec >= self.m_sec and  temp_tl.tm_sec <= (self.m_sec + last_second -1):				
				self.dawnOperateType_2(timestamp)
				print "------------------dawnOperate -> dawnOperateType_2"

			else:
				pass

		elif self.m_db_type == 3:		#月		
			if temp_tl.tm_mday == 1 and temp_tl.tm_hour == self.m_hour and temp_tl.tm_min == self.m_min and temp_tl.tm_sec >= self.m_sec and  temp_tl.tm_sec <= (self.m_sec + last_second -1):			
				self.dawnOperateType_3(timestamp)
				print "------------------dawnOperate -> dawnOperateType_3"
			else:
				pass

		else:
			pass

	def loop(self,timestamp,last_second):
		if self.m_db_status != 1:
			return

		self.dawnOperate(timestamp,last_second)

		self.m_timestamp = timestamp
		str_limit_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(timestamp - last_second ))
			
		temp_min_str = ""
		if self.m_min_value == -9999999:
			pass
		else:
			# if self.m_db_rule_a == "asc":
			# 	temp_min_str = " and "+ self.m_db_factor_a + "<" + str(self.m_min_value)
			# else:
			# 	temp_min_str = " and "+ self.m_db_factor_a + ">" + str(self.m_min_value)
			pass
			

		sql_str = "select userID," + self.m_db_factor_a + " from " + self.m_db_db_name + "\
				 where 1=1  and gameID ="+str(self.m_db_game_id) +" and editTime>='" + str_limit_time + "' and \
				  " + self.m_db_where_a + temp_min_str + "  \
				 limit " + str(self.m_db_sort_limit)

		#print "rank_id:",self.m_db_rank_id,"--",sql_str
		#temp_sql = sql_str
		self.m_db.excuteSql(sql_str)
		results = self.m_db.fetchall()

		i = 0	
		j = 0	#掉出m_db_sort_limit的高分人
		for row in results:	
			if self.m_unsort_dic.has_key(row[0]) == True and self.m_min_value != -9999999: 	
				#print "--------------------------", row[1] ,"----min:",self.m_min_value		
				if self.m_db_rule_a == "asc":
					if  row[1] >=  self.m_min_value:
						j = j + 1
						del self.m_sort_dic[ row[0] ]
					else:
						pass
				else:
					if row[1] <=  self.m_min_value:
						j = j + 1
						del self.m_sort_dic[ row[0] ]
					else:
						pass			
			else:
				pass
			self.m_unsort_dic[ row[0] ] = row[1]
			i = i + 1

		if j != 0:
			temp_min_str = ""
			if self.m_db_rule_a == "asc":
				temp_min_str = " and "+ self.m_db_factor_a + ">=" + str(self.m_min_value)
			else:
				temp_min_str = " and "+ self.m_db_factor_a + "<=" + str(self.m_min_value)

			sql_str = "select userID," + self.m_db_factor_a + " from " + self.m_db_db_name + " \
				 where 1=1 and  gameID =" + str(self.m_db_game_id) +" and " + self.m_db_where_a + temp_min_str + " and editTime>='" + self.str_type_limit_time + "' \
				order by "+ self.m_db_factor_a + " " + self.m_db_rule_a + " \
				limit " + str( self.m_db_sort_limit )#这里一定要self.m_db_sort_limit。因为最后一名的分数可能和它左右两边的人的分数一样。
			#print sql_str
			self.m_db.excuteSql(sql_str)
			results = self.m_db.fetchall()			
			for row in results:
				self.m_unsort_dic[ row[0] ] = row[1]					 
		else:
			pass

		if 0 != i:		
			#print "rank_id:",self.m_db_rank_id,"--",temp_sql	
			self.sortData()
		else:
			pass

	def sortData(self):
		sorted_list = []
		if self.m_db_rule_a == "asc":
			sorted_list = sorted(self.m_unsort_dic.iteritems(),key=lambda d:d[1])
		elif self.m_db_rule_a == "desc":
			sorted_list = sorted(self.m_unsort_dic.iteritems(),key=lambda d:d[1],reverse=True)
		else:
			pass

		self.moveSortToRank(sorted_list)	

	def moveSortToRank(self,sorted_list):
		length = len(sorted_list)
		for i in range(length):
			if i < self.m_db_sort_limit :
				self.insertRankDict(sorted_list[i][0],sorted_list[i][1],i+1)
				if (i+1) ==  self.m_db_sort_limit:
					self.m_min_value = sorted_list[i][1]
				else:
					pass
			else:
				del self.m_unsort_dic[ sorted_list[i][0] ]	#删除大于m_db_sort_limit的数据

	def insertRankDict(self,userID,value,rank):
		i = 0
		if self.m_sort_dic.has_key(userID) == True:	#是否存在这个userid
			if self.m_sort_dic[userID][0] != rank:	#排名是否有改
				self.m_sort_dic[userID] = [rank,value]
				self.updataSql(userID,value,rank)
				i = i + 1 
			else:
				pass
		else:
			self.m_sort_dic[userID] = [rank,value]
			self.updataSql(userID,value,rank)
			i = i + 1 

		#ko.rank.log.[gameid].last_update_time
		#ko.rank.log.[gameid].update_set
		#由于这里的redis操作都很简单，不会出现语句上的错误，所以，redis操作出问题了，那就重连redis，重连失败则挂掉脚本。
		if i > 0:
			self.m_redis.set("ko.rank.log." + str(self.m_db_game_id) + ".last_update_time",self.m_timestamp)
			self.m_redis.set("ko.rank.log." + str(self.m_db_game_id) + ".update_set",1)
		else:
			pass

	def updataSql(self,userID,value,rank):
		time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(self.m_timestamp))

		sql_str = "insert into "+self.m_insert_table
		sql_str += "(rankID,userID,rank,value,inputTime) values"
		sql_str += "("
		sql_str += str(self.m_db_rank_id) + ","
		sql_str += str(userID) + ","
		sql_str += str(rank) + ","
		sql_str += str(value) + ","
		sql_str += "'"+time_str+"'"
		sql_str += ") "
		sql_str += " on duplicate key update"
		sql_str += " userID="+str(userID)+","
		sql_str += " value="+str(value)+","
		sql_str += " inputtime=" + "'"+time_str+"';"

		#print sql_str
		self.m_db.excuteSql(sql_str)

	def dawnOperateType_0(self,timestamp):#每日
		self.m_min_value	= -9999999

		table_name = "battle_item_rank_history_" + time.strftime("%Y_%m_%d", time.localtime(timestamp))
		self.m_insert_table = "ko_rank." + table_name

		sql_str = "create table if not exists " + self.m_insert_table + " like ko_rank.battle_item_rank_history_YYYY_MM_DD;"
		self.m_db.excuteSql(sql_str)

	def dawnOperateType_1(self,timestamp):#历史
		pass

	def dawnOperateType_2(self,timestamp):#周
		self.m_min_value	= -9999999

		table_name = "battle_item_rank_history_" + time.strftime("%Y_WW_", time.localtime(timestamp))
		week = datetime.date.today().isocalendar()[1]
		if week < 10:
			self.m_insert_table = "ko_rank." + table_name + "0" + str(week)
		else:
			self.m_insert_table = "ko_rank." + table_name  + str(week)
		

		sql_str = "create table if not exists " + self.m_insert_table + " like ko_rank.battle_item_rank_history_YYYY_WW;"
		self.m_db.excuteSql(sql_str)
		

	def dawnOperateType_3(self,timestamp):#月
		self.m_min_value	= -9999999

		table_name = "battle_item_rank_history_" + time.strftime("%Y_%m", time.localtime(timestamp))
		self.m_insert_table = "ko_rank." + table_name

		sql_str = "create table if not exists " + self.m_insert_table + " like ko_rank.battle_item_rank_history_YYYY_MM;"
		self.m_db.excuteSql(sql_str)


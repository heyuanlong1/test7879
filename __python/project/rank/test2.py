#!/usr/bin/python
# -*- coding:UTF-8 -*-

"""

"""

import time
import math
import random
from RankDB import RankDB 
import time, os, sched 

m_db = RankDB()
inc_second = 1


def work():
	tt = time.time();
	tt = math.ceil(tt)
	for x in xrange(20,50):
		sql_str = "INSERT INTO ko_rank_config.`rank_config` VALUES ("+ str(x) +","+ str(x + 1000) +",'历史榜',1,0,'ko_user_own.user_property','value','dec','propertyID=103',500,'对战积分','根据每个玩家的天梯做排名','all','',1,'0000-00-00 00:00:00','','0000-00-00 00:00:00','')"
		print sql_str
		m_db.excuteSql(sql_str)



work()


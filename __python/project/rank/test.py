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
	for x in xrange(1,700):
		sql_str = "insert into ko_user_own.user_property(propertyID,userID,value,gameid,inputTime,editTime) values("
		sql_str += "100,"
		sql_str += str(random.randint(100000,110000) ) + ","
		sql_str += str(random.randint(-1000,-1000)) + ",102004,"
		sql_str += "now(),now())"
		sql_str += " on duplicate key update"
		sql_str += " value=" + str(random.randint(-1000,-1000)) + ","
		sql_str += " editTime=now();"
		print sql_str
		m_db.excuteSql(sql_str)


#定时执行work
schedule = sched.scheduler(time.time, time.sleep) 
def perform_command(cmd, inc): 
    schedule.enter(inc, 0, perform_command, (cmd, inc))
    time_beg = time.time()

    work()

    time_end = time.time()
    print "time_beg",time_beg
    print "consume time",time_end - time_beg

def timming_exe(cmd, inc ): 
    schedule.enter(inc, 0, perform_command, (cmd, inc)) 
    schedule.run() 

#timming_exe("", inc_second)
work();


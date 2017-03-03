#!/usr/bin/python
# -*- coding: utf-8 -*-

import MySQLdb
import time, os, sched ,random

MYSQL_HOST      ='#####'
MYSQL_PORT      =3306
MYSQL_USER      ='root'
MYSQL_PASSWORD  ='#####'



file = open("nn.txt")
x = 0
arr = []
while 1:
    line = file.readline()
    if not line:
        break
    pass
    line = line.replace('\r', '') 
    line = line.replace('\n', '')
    x = x +1 
    arr.append(line)


length = len(arr) - 1
for a in arr:
    print a


sql_connect=MySQLdb.connect(host=MYSQL_HOST,port=MYSQL_PORT,user=MYSQL_USER,passwd=MYSQL_PASSWORD) 
sql_connect.autocommit(1)
cursor = sql_connect.cursor() 

for x in xrange(0,30000):
	sql_str = "update ko_user.user set nickname='" +arr[random.randint(0, length)]+ "' where userid=" + str(x + 2500000)
	cursor.execute(sql_str)

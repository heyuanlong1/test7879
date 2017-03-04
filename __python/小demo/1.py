import MySQLdb
import time, os, sched 

MYSQL_HOST      ='#####'
MYSQL_PORT      =3306
MYSQL_USER      ='root'
MYSQL_PASSWORD  ='#####'



sql_connect=MySQLdb.connect(host=MYSQL_HOST,port=MYSQL_PORT,user=MYSQL_USER,passwd=MYSQL_PASSWORD) 
sql_connect.autocommit(1)
cursor = sql_connect.cursor() 


kk = 2500000
for x in xrange(0,30000):
	sql_str = "select nickName from ko_user.nick_name_box_new where seqid=" + str(x + 1215612)
	cursor.execute(sql_str)
	results = cursor.fetchall()			
	for row in results:
		sql_str = "update ko_user.user set nickname='" + row[0] + "' where userid=" + str(x + 2500000)
		cursor.execute(sql_str)

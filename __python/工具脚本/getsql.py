#!/usr/bin/python
# -*- coding: utf-8 -*-
"""

python getsql.py schema table


"""




import MySQLdb
import time, os, sched ,sys


##全局定义
MYSQL_HOST		='localhost'
MYSQL_PORT		=3306
MYSQL_USER		='root'
MYSQL_PASSWORD	='CQMYG14dss'
TOP_N 			=10



sql_connect=MySQLdb.connect(host=MYSQL_HOST,port=MYSQL_PORT,user=MYSQL_USER,passwd=MYSQL_PASSWORD) 
cursor = sql_connect.cursor() 
cursor.execute('SET NAMES UTF8')

def generate_insert(schema,table,list):
	#strstr =  "insert into "+schema+"."+table+ " values();"
	#print strstr

	strstr =  "insert into "+schema+"."+table+ "("
	max_i = len(list) - 1
	for i in range(len(list)):
		strstr += list[i][0]
		if i != max_i:
			strstr += ","
		else:
			pass
	strstr += ") values("

	for i in range(len(list)):
		if "char" in list[i][2]:
			strstr += "\""

		if list[i][1] is not None:
			strstr += list[i][1]	
		else:
			strstr += "0"
		
		if "char" in list[i][2]:
			strstr += "\""

		if i != max_i:
			strstr += ","
		else:
			pass
	strstr += ");"		
	print strstr


def generate_update(schema,table,list):
	strstr =  "update set  "+schema+"."+table+ " where ;"
	print strstr

def generate_delete(schema,table,list):
	strstr =  "delete  * from "+schema+"."+table+ " where ;"
	print strstr

def generate_select(schema,table,list):
	strstr =  "select  * from "+schema+"."+table+ " limit 100;"
	#print strstr

	max_i = len(list) - 1
	strstr = ""
	for i in range(len(list)):
		strstr += list[i][0]
		if i != max_i:
			strstr += ","
		else:
			pass
		if i % 3 == 0:
			strstr += "\n"

	print "select "+ strstr + " from "+schema+"."+table+" limit 100;\n"

	if len(sys.argv) > 3:
		sql_str = "select "+ strstr + " from "+schema+"."+table + " where "+ sys.argv[3]
		cursor.execute(sql_str)
		results = cursor.fetchall()
		for row in results:
			tmpstr=""
			for i in range(len(row)):
				if "char" in list[i][2]:
					tmpstr += "'" + str(row[i]) + "'" 
				else:
					tmpstr += str(row[i])
				if i != max_i:
					tmpstr += ","
				else:
					pass
				if i % 3 == 0:
					tmpstr += "\n"
			print  "insert into "+schema+"."+table+"("+ strstr+") \nvalues("+tmpstr+");"
		#print len(results)

def work_generate(schema,table,list):
	generate_select(schema,table,list)
	print "\n"
	generate_insert(schema,table,list)
	print "\n"

	#print list

def work_str(schema,table):
	column_list = []
	sql_str = "select column_name,column_default,data_type from information_schema.columns where table_schema='"+schema+"' and table_name='"+table+"'";
	cursor.execute(sql_str)
	results = cursor.fetchall()
	for row in results:
		temp_list = [ row[0],row[1],row[2] ]
		column_list.append(temp_list)

	work_generate(schema,table,column_list)


if sys.argv[1] is None or sys.argv[2] is None :
	print "2 arg"
else:
	schema = sys.argv[1]
	table = sys.argv[2]
	print ""
	work_str(schema,table)

cursor.close()
sql_connect.close()

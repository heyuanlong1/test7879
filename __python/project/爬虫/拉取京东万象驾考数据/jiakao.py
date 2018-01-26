#!/usr/bin/python
# -*- coding:UTF-8 -*-

import urllib
import urllib2
import re
import  thread
import time
import json
import MySQLdb
from MySQLDB import RankDB 

class QSBK:

    def __init__(self):
        self.user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
        self.headers = {'User-Agent' :self.user_agent}
        self.m_db = RankDB()
        self.sql_str = ""
        self.url = ""

    def getPage(self,pageIndex):
        try:
            self.url = 'https://way.jd.com/jisuapi/driverexamQuery?type=C1&subject=1&pagesize=20&pagenum='+ str(pageIndex) +'&sort=normal&appkey=xxxxxxxxxxxxxx'
            request = urllib2.Request(self.url,headers=self.headers)
            response = urllib2.urlopen(request)
            pageCode = response.read()
            return pageCode
        except urllib2.URLError,e:
            if hasattr(e,"reason"):
                print "error",e.reason
                return None

    def getPageItems(self,pageIndex):
        pageCode = self.getPage(pageIndex)
        if not pageCode:
            print "page load error"
            return None

        obj = json.loads(pageCode)
        print obj
        obj2 = obj["result"]["result"]
        pagenum =obj2["pagenum"]
        pagesize =obj2["pagesize"]
        subject =obj2["subject"]
        mtype =obj2["type"]

        for item in obj2["list"]:
            question = item["question"]
            option1 = item["option1"]
            option2 = item["option2"]
            option3 = item["option3"]
            option4 = item["option4"]
            answer = item["answer"]
            explain = item["explain"]
            pic = item["pic"]
            qtype = item["type"]
            chapter = item["chapter"]

            sql_str = "insert into gaoxiao.jiakao1"
            sql_str += "(pagenum,pagesize,subject,`type`,question,option1,option2,option3,option4,answer,`explain`,pic,qtype,chapter) values"
            sql_str += "("
            sql_str += pagenum + ","
            sql_str += pagesize + ","
            sql_str += "'"+subject+"',"
            sql_str += "'"+mtype+"',"
            sql_str += "'"+question+"',"
            sql_str += "'"+option1+"',"
            sql_str += "'"+option2+"',"
            sql_str += "'"+option3+"',"
            sql_str += "'"+option4+"',"
            sql_str += "'"+answer+"',"
            sql_str += "'"+explain+"',"
            sql_str += "'"+pic+"',"
            sql_str += "'"+qtype+"',"
            sql_str += "'"+chapter+"'"
            sql_str += ") "
            self.m_db.excuteSql(sql_str)

    def start(self):
        for x in range(1,65):
            time.sleep(1)
            expt = 0
            for j in xrange(1,1000):
                try:
                    self.getPageItems(x)
                except Exception, e:
                    expt = 1
                if expt == 0:
                    break
                expt = 0
                time.sleep(1)

            
        

spider = QSBK()
spider.start()


'''
CREATE TABLE `jiakao1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pagenum` int,
  `pagesize` int,
  `subject` varchar(50) DEFAULT '',
  `type` varchar(50) DEFAULT '',
  `question` text DEFAULT '',
  `option1` text DEFAULT '',
  `option2` text DEFAULT '',
  `option3` text DEFAULT '',
  `option4` text DEFAULT '',
  `answer` text DEFAULT '',
  `explain` text DEFAULT '',
  `pic` text DEFAULT '',
  `qtype` text DEFAULT '',
  `chapter` text DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=42 DEFAULT CHARSET=utf8;

'''

# coding=utf-8
import urllib
import urllib2
import re
import  thread
import time

from MySQLDB import MySQLDB 

class QSBK:

    def __init__(self):
        self.user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'
        self.headers = {'User-Agent' :self.user_agent}
        self.m_db = MySQLDB()
        self.sql_str = ""

    def getPage(self,pageIndex):
        try:
            url = 'http://www.qiushibaike.com/text/page/' + str(pageIndex)
            request = urllib2.Request(url,headers=self.headers)
            response = urllib2.urlopen(request)
            pageCode = response.read().decode('utf-8')
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
        pattern = re.compile('h2>(.*?)</h2.*?content">.*?<span>(.*?)</.*?number">(.*?)</',re.S)
        items = re.findall(pattern,pageCode)
        pageStories = []
        for item in items:
            mstr = item[1].strip().replace("<br/>", "\n",10);
            #print "\n" + mstr
            self.sql_str = "insert into gaoxiao.qiushibaike(text) values("+mstr+");"
            self.m_db.excuteSql(sql_str)

    def getOneStory(self,pageStories,page):
        print u"第%d页\t发布人：%s\t 赞：%s\n%s" %(page,story[0],story[2],story[1])

    def start(self):
        for x in xrange(1,2):
            self.getPageItems(1)
        

spider = QSBK()
spider.start()


'''
create table gaoxiao.qiushibaike(
`id` int(11) NOT NULL AUTO_INCREMENT,
`text` varchar(500) DEFAULT '',
PRIMARY KEY (`id`)
)ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

'''
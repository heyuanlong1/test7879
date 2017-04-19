# coding=utf-8
import urllib
import urllib2
import re
import  thread
import time

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
            self.url = 'http://www.qiushibaike.com/text/page/' + str(pageIndex)
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
        pattern = re.compile('h2>(.*?)</h2.*?content">.*?<span>(.*?)</.*?number">(.*?)</',re.S)
        items = re.findall(pattern,pageCode)
        pageStories = []
        for item in items:
            mstr = item[1].strip().replace("<br/>", "\n",10);
            #print "\n" + mstr
            #print type(mstr)
            #print type(item[2])
            self.sql_str = "insert into gaoxiao.qiushibaike(text,likes,type,inputTime) values('"+mstr+"',"+item[2]+",0,now() );"
            self.m_db.excuteSql(self.sql_str)

    def getOneStory(self,pageStories,page):
        print u"第%d页\t发布人：%s\t 赞：%s\n%s" %(page,story[0],story[2],story[1])

    def start(self):
        for x in range(1,35):
            time.sleep(1)
            self.getPageItems(x)
        

spider = QSBK()
spider.start()


'''
CREATE TABLE `qiushibaike` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` text,
  `inputTime` varchar(19) DEFAULT '' COMMENT '插入时间',
  `likes` int(11) DEFAULT '0' COMMENT '点赞数',
  `type` int(11) DEFAULT '0' COMMENT '0:纯文本  1:文本带图片 2:文本带媒体',
  `from` varchar(255) DEFAULT '' COMMENT '来源',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=42 DEFAULT CHARSET=utf8;

'''

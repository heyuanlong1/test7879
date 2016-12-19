# -*- coding:utf-8 -*-
import urllib
import urllib2
import re
import os
import Queue

class Spider:
    def __init__(self):
        self.siteurl = "http://www.mmjpg.com/"
        self.i = 1
        self.urlqueue = Queue.Queue(maxsize=10000000000)
        self.simg = set()
        self.surl =set()
        self.urlqueue.put(self.siteurl)
    def getPage(self):
        url = self.urlqueue.get()
        request = urllib2.Request(url)
        response = urllib2.urlopen(request)
        return response.read()
    def getContents(self):
        page = self.getPage();
        pattern = re.compile('<a href="(.*?)".*?><img src="(.*?)".*?/>',re.S);
        items = re.findall(pattern,page);
        contents = []
        for item in items:
            contents.append([item[0],item[1]])
        return contents;
    def saveImg(self,imgurl):
        if imgurl not in self.simg:
            self.simg.add(imgurl)
            u = urllib.urlopen(imgurl)
            data = u.read();
            f = open("./generatefile/"+str(self.i)+".jpg",'wb+')
            f.write(data)
            print "save..."
            f.close();
            self.i = self.i + 1;
            if(self.i > 5):
                exit()
    def savePageInfo(self):
        contents = self.getContents();
        for item in contents:
            self.saveImg(item[1])
            if item[0] not in self.surl:
                self.surl.add(item[0])
                self.urlqueue.put(item[0])
        self.savePageInfo()

spider = Spider()
spider.savePageInfo()


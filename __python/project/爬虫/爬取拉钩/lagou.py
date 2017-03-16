# -*- coding:utf-8 -*-
import urllib
import urllib2
import re
import os
import Queue



def main():
    files = open("11.txt")
    arr1 = []
    while 1:
        line = files.readline()
        if not line:
            break
        pass
        line = line.replace('\r', '') 
        line = line.replace('\n', '')
        arr1.append(line)
    files.close()
    
    files = open("22.txt")
    arr2 = []
    while 1:
        line = files.readline()
        if not line:
            break
        pass
        line = line.replace('\r', '') 
        line = line.replace('\n', '')
        arr2.append(line)
    files.close()

    f=file("33.txt","a+")
    f.truncate()
    for yuyan in arr1:
        for year in arr2:
            url = "https://www.lagou.com/jobs/list_"+yuyan+"?px=default&gj="+year+"&city=深圳#"
            print url
            f.write(url+"\n")
    f.close()

main()


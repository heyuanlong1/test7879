#!/bin/bash

#
# 崩溃重启脚本
# 加入contab 一分钟跑一次  * * * * * /data/scripts/check_MatchVS_python.sh
#
#



check_array=(MatchVSrank MainDayRank MainMostScore MainHistoryRank rank1.8/MainRank)
start_arg="nohup python /data/scripts/rank2/"
log_arg="/data/scripts/rank2/"

stout="2>&1"

for i in  ${check_array[*]}
    do  
        status=`ps -ef|grep ${i}|grep -v grep |awk '{print $2}'`
        if [ ${status} ] && [ -d  "/proc/`ps -ef|grep ${i}|grep -v grep |awk '{print $2}'`" ];then
            echo "$i is running."
        else
            echo "starting $i ..."
            if [ ${i}x = "MatchVSrank"x ];then
                nohup python /data/scripts/rank/MatchVSrank.py >> /data/scripts/rank/MatchVSrank.log 2>&1 &
            elif  [ ${i}x = "rank1.8/MainRank"x ];then
                nohup python /data/scripts/rank1.8/MainRank.py >> /data/scripts/rank1.8/log.txt 2>&1 &
            else
                ${start_arg}${i}.py >> ${log_arg}${i}.log ${stout} &
            fi  
        fi  


    done

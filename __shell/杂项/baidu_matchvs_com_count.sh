#!/bin/bash


#
#
#
#  加入crontab 每十分钟执行一次  
#  */10 * * * * /../baidu_matchvs_com_count.sh > /../baidu_matchvs_com_count.log 2>&1
#


#生成随机数[min max)
function random()
{
	min=$1
	max=$2-$1
	num=$RANDOM

	((retnum=num%max+min));
	echo $retnum;

}

#宣传期 1/5  1/19
xuan_start=`date -d"2016-01-05 00:00:00" +%s`
xuan_end=`date -d"2016-01-20 00:00:00" +%s`
#投票阶段 1/20   2/6
tou_start=`date -d"2016-01-20 00:00:00" +%s`
tou_end=`date -d"2016-02-07 00:00:00" +%s`
#过年期间2/7  2/15
guo_start=`date -d"2016-02-07 00:00:00" +%s`
guo_end=`date -d"2016-02-16 00:00:00" +%s`
#投票最终时间2/16  2/29
zhong_start=`date -d"2016-02-16 00:00:00" +%s`
zhong_end=`date -d"2016-03-01 00:00:00" +%s`
#当前时间戳
now_date=`date +%s`
#时
h_date=`date +"%H"`

#judge 用于判断是否要增加。 0:inc为1  1：inc为0
judge=0
inc=0


#宣传期	 2   1   3
if [ $now_date -gt $xuan_start ] && [ $now_date -lt $xuan_end ]; then
	if [ $h_date -ge 9 ] && [ $h_date -le 12 ]; then
		judge=$(random 0 3);
	elif [ $h_date -eq 13 ]; then
		judge=$(random 0 6);
	elif [ $h_date -ge 14 ] && [ $h_date -le 20 ]; then
		judge=$(random 0 2);
	else
		judge=1;
	fi
	echo "xuanchuan"
fi

#投票阶段	3	6	12
if [ $now_date -gt $tou_start ] && [ $now_date -lt $tou_end ]; then
	if [ $h_date -ge 9 ] && [ $h_date -le 12 ]; then
		judge=$(random 0 2);
	elif [ $h_date -eq 13 ]; then
		judge=1;
		inc=1;
	elif [ $h_date -ge 14 ] && [ $h_date -le 20 ]; then
		judge=1;
		inc=2;
	else
		judge=1;
	fi	
	echo "toupiao"
fi

#过年期间 1
if [ $now_date -gt $guo_start ] && [ $now_date -lt $guo_end ]; then
	if [ $h_date -ge 14 ] && [ $h_date -le 20 ]; then
		judge=$(random 0 6);
	else
		judge=1;
	fi	
	echo "guonian"
fi

#投票最终时间 3  	6	12
if [ $now_date -gt $zhong_start ] && [ $now_date -lt $zhong_end ]; then
	if [ $h_date -ge 9 ] && [ $h_date -le 12 ]; then
		judge=$(random 0 2);
	elif [ $h_date -eq 13 ]; then
		judge=1;
		inc=1;
	elif [ $h_date -ge 14 ] && [ $h_date -le 20 ]; then
		judge=1;
		inc=2;
	else
		judge=1;
	fi	
	echo "zuizhong"
fi

echo "--judge:"$judge
if [ $judge -eq 0 ]; then
	inc=1;
fi

red="/usr/local/redis/bin/redis-cli"

now_nums=`$red<<EOF
auth FDFDfdi4k25e@sf
get matchvs_inc_count
EOF`

nums=`echo $now_nums  | awk '{print int($2)}'`

echo "--inc:"$inc
nums=$[ $nums+$inc ]

$red<<EOF
auth FDFDfdi4k25e@sf
set matchvs_inc_count $nums
EOF

echo "please enter gameid"
read gameid1
echo "please enter gameid again"
read gameid2

if [[ $gameid1 == $gameid2 ]];then
        echo $gameid1"=="$gameid2
else
		echo $gameid1"!="$gameid2
        exit
fi


mysql39="/usr/local/mysql/bin/mysql --login-path=remote227"
mysqldump39="/usr/local/mysql/bin/mysqldump --login-path=remote227"

${mysqldump39} ko_currency user_product > user_product.sql
${mysqldump39} ko_battle battle_score > battle_score.sql
${mysqldump39} ko_battle battle_item_rank_history > battle_item_rank_history.sql


select_sql="delete  from ko_currency.user_product where gameid=${gameid2} and userid not in(select userid from ko_user.auto_user)"
${mysql39}  -e "${select_sql}"

select_sql="delete  from ko_battle.battle_score where gameid=${gameid2}"
${mysql39}  -e "${select_sql}"

select_sql="delete  from ko_battle.battle_item_rank_history where gameid=${gameid2}"
${mysql39}  -e "${select_sql}"


select_sql="select * from ko_currency.user_product where gameid=${gameid2} and userid not in(select userid from ko_user.auto_user)"
res=`${mysql39}  -e "${select_sql}"`
echo "user_product ${gameid2}"
echo ${res}

select_sql="select * from ko_battle.battle_score where gameid=${gameid2}"
res=`${mysql39}  -e "${select_sql}"`
echo "battle_score ${gameid2}"
echo ${res}

select_sql="select * from ko_battle.battle_item_rank_history where gameid=${gameid2}"
res=`${mysql39}  -e "${select_sql}"`

echo "battle_item_rank_history ${gameid2}"
echo ${res}


#ÖØÆôÅÅÐÐ½Å±¾
pid=`ps aux | grep  /data/scripts/rank2/MainHistoryRank.py | awk '{if($11=="python"){print $2}}'`
kill -9 ${pid}
nohup python /data/scripts/rank2/MainHistoryRank.py > /data/scripts/rank2/MainHistoryRank.log 2>&1 &

pid=`ps aux | grep  /data/scripts/rank2/MainDayRank.py | awk '{if($11=="python"){print $2}}'`
kill -9 ${pid}
nohup python /data/scripts/rank2/MainDayRank.py > /data/scripts/rank2/MainDayRank.log 2>&1 &



pid=`ps aux | grep  /data/scripts/rank2/MainHistoryRank.py | awk '{if($11=="python"){print $2}}'`
echo "MainHistoryRank.py pid"
echo ${pid}
pid=`ps aux | grep  /data/scripts/rank2/MainDayRank.py | awk '{if($11=="python"){print $2}}'`
echo "MainDayRank.py pid"
echo ${pid}
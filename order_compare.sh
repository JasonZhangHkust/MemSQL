#!/bin/bash


if [ "$#" -eq 4 ];then
    now=$3" "$4":0:0"
    use_time=1
elif [ "$#" -eq 2 ]
then
    use_time=0 
	#now=$(date "+%Y-%m-%d %H")":0:0"
else
    echo "Usage:"
    echo "$0 file_name1 file_name2 year-mon-day hour"
    exit 1
fi

file_name1=$1
file_name2=$2

cat $file_name1 | awk -v filename="$file_name1" '{print $0",filename"}' > mergedAuditLog
cat $file_name2 | awk -v filename="$file_name2" '{print $0",filename"}' >> mergedAuditLog
output=$(date "+%Y-%m-%d_%H:%M")
if [ "$use_time" -eq 1 ];then
    sharptime=$(date -d "$now" +%s)
    echo $sharptime
    sort -k1,1 -t,  mergedAuditLog | awk -F, -v time="$sharptime" '{if($1>=time) {$1="";print $0}}' | sort -k9,9 -s | uniq -c  | awk '{if($1!=2 && $10 !~ /DelTask/) print $0}' > unMacthedAuditLog_"$output"
    awk 'BEGIN{print "UID" "\t\t" "SUM"}{sum[$2]++}END{{for(i in sum) print i "\t" sum[i]};print "TOTAL\t\t" length(sum)}' unMacthedAuditLog_"$output" > unMacthedStaticsbyUid_"$output"
else
    echo "From Begin"
    sort -k1,1 -t,  mergedAuditLog | awk -F, '{$1="";print $0}' | sort -k9,9 -s | uniq -c  | awk '{if($1!=2 && $10 !~ /DelTask/) print $0}' > unMacthedAuditLog_"$output"
    awk 'BEGIN{print "UID" "\t\t" "SUM"}{sum[$2]++}END{{for(i in sum) print i "\t" sum[i]};print "TOTAL\t\t" length(sum)}' unMacthedAuditLog_"$output" > unMacthedStaticsbyUid_"$output"
    exit -1
fi

#cat $file_name1 | awk -F, '{$1="";print $0}' > mergedAuditLog
#cat $file_name2 | awk -F, '{$1="";print $0}' >> mergedAuditLog
#先按时间排序，然后筛选出具有相同订单号其他参数却不同的记录，找出每个uid最早的不匹配记录
exit 0

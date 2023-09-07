#!/bin/sh
# git fsck --lost-found >> 1.txt
# excute this script
# get right commit id
# git stash apply commitid
READFILENAME="1.txt"      #上步文件名【1.txt dangling commit，2.txt对应unreachable commit】
RETFILENAME="./ret.txt"   #筛选结果文件名
while read -r line
do
    VAR=`echo $line | awk '/dangling commit/ {print $3}'`
    if [ $VAR ]; then
        git show $VAR >> $RETFILENAME
        echo "-------------------------------------\n" >>$RETFILENAME #每条记录用---分割开
    fi
done < $READFILENAME

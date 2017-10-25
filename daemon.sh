#!/bin/sh

COMDIR=`pwd`

ulimit -c unlimited
protect ()
{
    dpcid=$(ps -elf|grep -v "grep"|grep "$1"$| awk '{print $4}'|wc -l)
    echo $dpcid

    if [ "$dpcid" -lt $3 ];
    then
        echo "进程不存在,重启进程!"     
        cd $2
        nohup $1>/dev/null &
    fi
}

while true;
do

    protect "./DataTransfer" ${COMDIR} 1
    #protect "./CommonCollection" ${COMDIR} 
    sleep 5

done

#!/bin/sh

COMDIR=`pwd`

#source ~/.bashrc

daemonSh=daemon.sh


CallStart()
{
    if [ ! -f $daemonSh ];then
    echo "缺少daemon.sh"
    exit
    fi

    echo "正在启动..."
    status=`ps -elf | grep ${daemonSh}|grep -v grep`
    if [ "-h" == "-h${status}" ];then
                cd $COMDIR
                nohup ./$daemonSh >/dev/null &
                sleep 1
    fi

    echo "启动成功"
}

CallStop()
{

    echo "正在停止数据传输系统..."
    killall daemon.sh
    killall -15 DataTransfer

    num=0
    while [ $num -lt 60 ]
    do
      s2=`ps -elf | grep DataTransfer|grep -v grep | grep -v DataTransfer`

      if [ "-" == "-${s2}" ];then
         break
      fi
          let "num++"
      sleep 1
    done

    echo "数据传输系统已停止"

}

#进入到Bin目录下执行
#source ~/.bashrc
cd ${COMDIR}  
case "$1" in
    start)
   
		#启动数据传输系统,调用cts_parallel_daemon.sh脚本
		CallStart

   ;;

    stop)
        #停止数据传输系统,
        CallStop


   ;;

   restart)
     $0 stop
         sleep 2
         $0 start
         ;;
         
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
        ;;
esac

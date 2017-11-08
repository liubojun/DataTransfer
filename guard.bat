@echo off

::检测时间间隔，单位：秒
set _interval=5

::需要守护的进程名称
set _processName=DataTransfer

::需要守护的进程启动命令
set _processCmd=DataTransfer.exe

::需要守护的进程预估启动完毕所需时间，单位：秒
set _processTimeout=10

::需要守护的进程所监听的端口
set _port=8080

::进程用户名，一般是Administrator
set _username=Administrator

:LOOP
set /a isAlive=false

::通过进程名称检测
tasklist /FI "username eq %_username%" | find /C "%_processName%" > temp.txt
set /p num= < temp.txt
del /F temp.txt


if "%num%" == "0" (
start %_processCmd% | echo 启动 %_processName% 于 %time%
choice /D y /t %_processTimeout% > nul
)

if "%num%" NEQ "0" echo 已启动

::ping -n %_interval% 127.1>nul
choice /D y /t %_interval% >nul

goto LOOP
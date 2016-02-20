@echo off
cd %~dp0
adb push %~dp0/../../libs/armeabi/RootService /data/local/tmp/
adb shell su -c "chmod 777 /data/local/tmp/RootService"
echo.
echo.
echo 'result of running:'
echo ===========================================================

:MENU
echo (0)exit script
echo (1)Runing
echo (2)Ptrace
echo (3)SandBox
echo (4)Hook
set /p input="please input you request:"
if '%input%' == '0'	(goto EXIT)
if '%input%' == '1'	(goto RUNING)
if '%input%' == '2'	(goto PTRACE)
if '%input%' == '3'	(goto SANDBOX)
if '%input%' == '4'	(goto HOOK)
echo.
:RUNING
	echo +++++++++++++++++++++ Runing Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 1 123456"
	echo.
	echo.
	pause && @goto MENU
:PTRACE
	echo +++++++++++++++++++++ Ptrace Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 2 123456"
	echo.
	echo.
	pause && @goto MENU
:SANDBOX
	echo +++++++++++++++++++++ SandBox Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 3 123456"
	echo.
	echo.
	pause && @goto MENU
:HOOK
	echo +++++++++++++++++++++ Hook Test +++++++++++++++++++++
	@rem adb shell su -c "/data/local/tmp/RootService 4 'pid=123' {'libc.so:access','libc.so:open'}"
	adb shell su -c "/data/local/tmp/RootService 4 'pkg=com.tomagoyaky.librarysecurity' 'libc.so=access,libc.so=open'"
	echo.
	echo.
	pause && @goto MENU
:EXIT
	echo 'exit ...'
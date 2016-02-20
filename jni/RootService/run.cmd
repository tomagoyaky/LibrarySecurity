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
echo (1)Runing test
echo (2)Ptrace test
echo (3)SandBox test
set /p input="please input you request:"
if '%input%' == '0'	(goto EXIT)
if '%input%' == '1'	(goto RUNING)
if '%input%' == '2'	(goto PTRACE)
if '%input%' == '3'	(goto SANDBOX)
echo.
:RUNING
	echo +++++++++++++++++++++ Runing Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 123456 1"
	echo.
	echo.
	pause && @goto MENU
:PTRACE
	echo +++++++++++++++++++++ Ptrace Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 123456 2 "
	echo.
	echo.
	pause && @goto MENU
:SANDBOX
	echo +++++++++++++++++++++ SandBox Test +++++++++++++++++++++
	adb shell su -c "/data/local/tmp/RootService 123456 3"
	echo.
	echo.
	pause && @goto MENU
:EXIT
	echo 'exit ...'
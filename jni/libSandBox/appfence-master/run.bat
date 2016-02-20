@echo off
adb push libs/armeabi/appfence /data/local/tmp/appfence

adb shell su -c "chmod 777 data/local/tmp/appfence"

adb shell su -c "data/local/tmp/appfence"

pause
#APP_MODULES := base

#APP_STL := gnustl_static
APP_STL :=stlport_static
APP_PLATFORM := android-15
#
#NDK_DEBUG:=0
#JNIDEBUG:=0
#
APP_ABI :=   armeabi armeabi-v7a x86
#
#ifeq ($(NDK_DEBUG),1)
#APP_OPTIM := debug
#OPT_CFLAGS := 
#
#else
#APP_OPTIM := release
#OPT_CFLAGS := -Os  
#
#endif
#
#ifneq ($(NDK_DEBUG), 1)
#NDK_TOOLCHAIN_VERSION :=clang3.5-ijiami
#endif
#
#OPT_CFLAGS += -Os
#
#ifeq ($(NDK_TOOLCHAIN_VERSION),clang3.5-ijiami)
#V:=0
##OPT_CFLAGS += -O0 -mllvm -cpi -mllvm -cpi-instnumber=4 \
#				-mllvm -cpi-mbbnumber=40 -mllvm -equ -mllvm -cxf \
#				-mllvm -fcf -mllvm -ecs 
##OPT_CFLAGS += -Os   -mllvm -cpi -mllvm -cpi-instnumber=5 \
#				-mllvm -cpi-mbbnumber=100 -mllvm -ecs -mllvm -fcf  -mllvm -equ
##OPT_CFLAGS += -mllvm -cpi -mllvm -cpi-instnumber=3 -mllvm -cpi-mbbnumber=100
##OPT_CFLAGS += -mllvm -cpi -mllvm -cpi-instnumber=3 -mllvm -cpi-mbbnumber=50 
#OPT_CFLAGS += -mllvm -ecs #瀛楃涓�
#OPT_CFLAGS += -mllvm -fcf #liuc
#OPT_CFLAGS += -mllvm -equ
##OPT_CFLAGS += -mllvm -cxf
#endif


APP_CFLAGS := $(APP_CFLAGS) $(OPT_CFLAGS) 
APP_CPPFLAGS := $(APP_CPPFLAGS) $(OPT_CPPFLAGS) 

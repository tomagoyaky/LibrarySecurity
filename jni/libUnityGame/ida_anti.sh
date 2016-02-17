#!/bin/sh

make clean

make

./so_jiami/strip/arm/strip ./libs/armeabi/libexecdllgame.so

./so_jiami/strip/arm/strip ./libs/armeabi-v7a/libexecdllgame.so

./so_jiami/strip/x86/strip ./libs/x86/libexecdllgame.so


./ida-anti ./libs/armeabi/libexecdllgame.so


./ida-anti ./libs/armeabi-v7a/libexecdllgame.so


./ida-anti ./libs/x86/libexecdllgame.so

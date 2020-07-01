#!/bin/bash

#QEMU=qemu-system-x86_64
QEMU=/home/gic4107/qemu/out/bin/qemu-system-x86_64
UBUNTU_ISO=/home/gic4107/Downloads/ubuntu-18.04-desktop-amd64.iso
#UBUNTU_ISO=/home/gic4107/Downloads/ubuntu-18.04.4-desktop-amd64.iso

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] || [ -z $4 ] || [ -z $5 ] || [ -z $6 ] || [ -z $7 ]; then 
    echo "arg1 as dsag enable; arg2 as local memory size (MB); arg3 as # of cores;
          arg4 as memory size; arg5 as disk; arg6 as port forwarding; arg7 as vnc port"
    exit
fi

DSAG_EN=$1
LOCAL_MEM=$2
CORES=$3
MEM=$4
DISK=$5
NETWORK_SETTING=$6
VNC_PORT=$7

echo $NETWORK_SETTING

if [ $DSAG_EN -eq 1 ]; then
    DSAG="-dsag-sim"
fi

$QEMU \
    -enable-kvm \
    -m $MEM \
    -hda $DISK \
    -smp $CORES \
    $NETWORK_SETTING \
    -cpu host \
    -vga virtio \
    $DSAG \
    -dsag-local-mem-size $LOCAL_MEM \
    -vnc :$VNC_PORT
    #-serial tcp::1234,server,nowait \
    #-cdrom $UBUNTU_ISO \
    #-dsag-network-delay 0 \
    #-append 'kgdbwait kgdboc=ttyS0,115200' \
    #-s

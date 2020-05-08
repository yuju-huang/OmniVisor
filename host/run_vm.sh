#!/bin/bash

QEMU=/home/gic4107/qemu/out/bin/qemu-system-x86_64
DISK=/home/gic4107/dsag/ubuntu1.qcow2
UBUNTU_ISO=/home/gic4107/Downloads/ubuntu-18.04-desktop-amd64.iso

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]; then 
    echo "arg1 as local memory size (MB); arg2 as # of cores; arg3 as memory size"
    exit
fi

$QEMU \
    -enable-kvm \
    -m $3 \
    -hda $DISK \
    -smp $2 \
    -net nic,model=virtio \
    -net user,hostfwd=tcp::2222-:22,hostfwd=tcp::4444-:11211 \
    -cpu host \
    -vga virtio \
    -vnc :1
    #-serial tcp::1234,server,nowait \
    #-dsag-sim -dsag-local-mem-size $1 \
    #-cdrom $UBUNTU_ISO \
    #-dsag-network-delay 0 \
    #-append 'kgdbwait kgdboc=ttyS0,115200' \
    #-s

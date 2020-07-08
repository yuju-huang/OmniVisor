#!/bin/bash

MOUNT_POINT=/media/ramdisk
DISK_PATH=${MOUNT_POINT}/nbd_ramdisk.swap

SIZE=40G
COUNT=8000000

if [ ! -f $DISK_PATH ]; then
    sudo umount $MOUNT_POINT
    sudo mount -t tmpfs -o size=$SIZE tmpfs $MOUNT_POINT
    dd if=/dev/zero of=$DISK_PATH bs=4k count=$COUNT
fi

$HOME/OmniVisor/host/nbdkit_upstream/out/sbin/nbdkit -p 9999 file file=$DISK_PATH

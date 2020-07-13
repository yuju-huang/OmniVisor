#!/bin/bash

MOUNT_POINT=/media/ramdisk
DISK_PATH=${MOUNT_POINT}/nbd_ramdisk.ext4

SIZE=5120M
COUNT=1000000

sudo umount $MOUNT_POINT
sudo mount -t tmpfs -o size=$SIZE tmpfs $MOUNT_POINT
dd if=/dev/zero of=$DISK_PATH bs=4k count=$COUNT
$HOME/OmniVisor/host/nbdkit_upstream/out/sbin/nbdkit file file=$DISK_PATH

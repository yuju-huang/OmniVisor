#!/bin/bash

sudo modprobe nbd
#sudo nbd-client 10.147.6.146 9999 -N ext4 /dev/nbd0
sudo /home/gic4107/nbd-3.20/nbd-client 10.147.6.146 9999 -N ext4 /dev/nbd0
sudo mount -o rw /dev/nbd0 /mnt/ext4

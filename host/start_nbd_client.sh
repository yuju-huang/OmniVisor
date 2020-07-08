#!/bin/bash

SERVER_IP=128.84.139.15
PORT=9999

sudo modprobe nbd
sudo $HOME/OmniVisor/host/nbd-3.20/nbd-client $SERVER_IP $PORT /dev/nbd0
#sudo $HOME/OmniVisor/host/nbd-3.20/nbd-client $SERVER_IP $PORT -N ext4 /dev/nbd0
#sudo mount -o rw /dev/nbd0 /mnt/ext4

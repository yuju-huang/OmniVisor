#!/bin/bash

SERVER_IP=128.84.139.15
PORT=9999
MOUNT_POINT=/dev/nbd0

sudo modprobe nbd
sudo $HOME/OmniVisor/host/nbd-3.20/nbd-client $SERVER_IP $PORT $MOUNT_POINT
sudo mkswap $MOUNT_POINT

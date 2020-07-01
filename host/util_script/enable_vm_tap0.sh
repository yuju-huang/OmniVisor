#!/bin/bash

# https://serverfault.com/questions/197156/set-tap0-using-virt-manager-for-bridged-wireless

sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo tunctl -t tap0
sudo ip link set tap0 up
sudo ip addr add 10.147.8.1/24 dev tap0
sudo route add -host 10.147.8.1 dev tap0
sudo parprouted enp0s31f6 tap0
#sudo parprouted wlp2s0 tap0

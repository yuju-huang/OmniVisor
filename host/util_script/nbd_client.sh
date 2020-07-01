#!/bin/bash

IP=$1
PORT=9999
NAME=swap

sudo nbd-client $IP $PORT -N swap /dev/nbd0

#!/bin/bash

sudo -i
echo never > /sys/kernel/mm/transparent_hugepage/enabled
exit

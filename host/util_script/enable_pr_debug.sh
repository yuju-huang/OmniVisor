#!/bin/bash

#sudo sh -c "echo -n 'file mmu.c line 4120 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 90 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 113 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 123 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 234 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file vmscan.c line 1153 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file vmscan.c line 1381 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file vmscan.c line 1445 +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file vmscan.c line 1467 +p' > /sys/kernel/debug/dynamic_debug/control"

#!/bin/bash

sudo sh -c "echo -n 'file mmu.c line 4120 -p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 29 -p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 180 -p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 192 -p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 82 -p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo -n 'file dsag_mem_simulation.c line 134 -p' > /sys/kernel/debug/dynamic_debug/control"

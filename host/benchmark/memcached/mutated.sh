#!/bin/bash

for (( i=0; i<5; i++ ))
do
    $HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 1572864 -v 4096 -u 0 10.147.8.2:11211 -s 120 20000 -n 16 -w 10 -c 10  # Get (6G data)
    # $HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 20 -z 1000000 -v 4096 -u 0 127.0.0.1:4441 -s 60 40000 -n 16 -w 10 -c 10
done

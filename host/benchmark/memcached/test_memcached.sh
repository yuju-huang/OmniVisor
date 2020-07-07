#!/bin/bash

GENERATOR=$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache
IP=127.0.0.1
PORT=4441

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]; then
    echo "arg1 as Get/Set; arg2 as dataset size; arg3 as # of Get operations; arg4 as log filename"
    exit
fi

CMD=$1
DATASET_SIZE=$2
NUM_GETS=$3

get() {
GET_TIME=30
GET_RPS=80000
    echo "cmd=get"
    if [ $DATASET_SIZE -eq "4G" ]; then
        EXEC="$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 20 -z 1000000 -v 4096 -u 0.1 127.0.0.1:4441 -s $GET_TIME $GET_RPS -n 16 # Get (4G data)"
    elif [ $DATASET_SIZE -eq "8G" ]; then
        EXEC="$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 2000000 -v 4096 -u 0.1 127.0.0.1:4441 -s $GET_TIME $GET_RPS -n 16 # Get (8G data)"
    else
        echo "not supported dataset size"
        exit 1
    fi

    for (( i=1; i<=$NUM_GETS; i++ ))
    do
        echo "times="$i
        # TODO: Get command with timeout
        bash $EXEC
        if [ $? -ne 0 ]; then
            i--
        fi
    done
}

set() {
SET_RPS=40000
    echo "cmd=set"
    if [ $DATASET_SIZE -eq "4G" ]; then
        $HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 20 -z 1000000 -v 4096 -u 9 127.0.0.1:4441 -s 40 $SET_RPS -n 16   # Set (4G data)
    elif [ $DATASET_SIZE -eq "8G" ]; then
        $HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 2000000 -v 4096 -u 9 127.0.0.1:4441 -s 120 $SET_RPS -n 16  # Set (8G data)
    else
        echo "not supported dataset size"
        exit 1
    fi
    RET=$?
    # TODO: Check size
}

if [ $CMD -eq "Get" ]; then
    get
else
    set
fi

#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 20 -z 1000000 -v 4096 -u 16 127.0.0.1:4441 -s 60 40000 -n 16   # Set (4G data)
#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 20 -z 1000000 -v 4096 -u 0 127.0.0.1:4441 -s 120 40000 -n 16 -w 10 -c 10   # Get (4G data)

#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 1572864 -v 4096 -u 16 10.147.8.2:11211 -s 120 20000 -n 16   # Set (6G data)
#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 1572864 -v 4096 -u 0 10.147.8.2:11211 -s 120 20000 -n 16 -w 10 -c 10  # Get (6G data)

#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 2000000 -v 4096 -u 16 127.0.0.1:4441 -s 120 40000 -n 16  # Set (8G data)
#$HOME/OmniVisor/host/mutated/out/bin/mutated_memcache -k 21 -z 2000000 -v 4096 -u 0 127.0.0.1:4441 -s 120 40000 -n 16 -w 10 -c 10   # Get (8G data)

#!/bin/bash

# REF: http://blog.vmsplice.net/2017/11/common-disk-benchmarking-mistakes.html

LIBNBD_PATH=/usr/local/lib/
URI="nbd://10.147.6.146:9999"

LOOP=10
IODEPTH=16
DIRECT=1
SIZE=4G
IOENGINE=libaio
TIME=60
WARMUP_TIME=10

BLOCK_SIZES="4k 8k 16k 32k 64k 128k"
READ_WRITES="read write randread randwrite"

# Benchmark both SSD and HDD.
test_normal() {
# Used to iterate different block device.
DIRECTORYS="/home/gic4107/OmniVisor/host/benchmark/fio/fio-tmp/ /home/gic4107/Data/fio-tmp/"

for DIRECTORY in $DIRECTORYS
do
    echo "path="$DIRECTORY
    for RW in $READ_WRITES
    do
        for BS in $BLOCK_SIZES
        do
            NAME=${RW}_${BS}
            echo "name="$NAME
            LD_LIBRARY_PATH=$LIBNBD_PATH fio --directory=$DIRECTORY --name=$NAME --direct=1 --ramp_time=$WARMUP_TIME --time_based=1 --runtime=$TIME --rw=$RW --direct=$DIRECT --loops=$LOOP --size=$SIZE --ioengine=$IOENGINE --iodepth=$IODEPTH --bs=$BS
            rm ${DIRECTORY}/${NAME}.0.0
        done
    done
done
}

# Benchmark NBD.
test_nbd() {
echo "path=nbd"
for RW in $READ_WRITES
do
    for BS in $BLOCK_SIZES
    do
        NAME=${RW}_${BS}
        echo "name="$NAME
        LD_LIBRARY_PATH=$LIBNBD_PATH fio --name=$NAME --direct=1 --ramp_time=$WARMUP_TIME --time_based=1 --runtime=$TIME --rw=$RW --direct=$DIRECT --loops=$LOOP --size=$SIZE --ioengine=nbd --iodepth=$IODEPTH --bs=$BS --uri=$URI
    done
done
}

test_normal
test_nbd

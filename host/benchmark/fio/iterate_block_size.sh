#!/bin/bash

# REF: http://blog.vmsplice.net/2017/11/common-disk-benchmarking-mistakes.html

FIO=$HOME/OmniVisor/host/fio_upstream/out/bin/fio

LIBNBD_PATH=/usr/local/lib/

ETHERNET_ADDR=nbd://128.84.139.15:9999
INFINIBAND_ADDR=nbd://192.168.99.21:9999
HDD_PATH=$HOME/OmniVisor/host/benchmark/fio/fio-tmp/
SSD_PATH=/mnt/sdb

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
DIRECTORYS="$SSD_PATH $HDD_PATH"

for DIRECTORY in $DIRECTORYS
do
    echo "path="$DIRECTORY
    for RW in $READ_WRITES
    do
        for BS in $BLOCK_SIZES
        do
            NAME=${RW}_${BS}
            echo "name="$NAME
            LD_LIBRARY_PATH=$LIBNBD_PATH $FIO --directory=$DIRECTORY --name=$NAME --direct=1 --ramp_time=$WARMUP_TIME --time_based=1 --runtime=$TIME --rw=$RW --direct=$DIRECT --loops=$LOOP --size=$SIZE --ioengine=$IOENGINE --iodepth=$IODEPTH --bs=$BS
            rm ${DIRECTORY}/${NAME}.0.0
        done
    done
done
}

# Benchmark NBD.
test_nbd() {
URIS="$ETHERNET_ADDR $INFINIBAND_ADDR"

for URI in $URIS
do
    echo "path="$URI
    for RW in $READ_WRITES
    do
        for BS in $BLOCK_SIZES
        do
            NAME=${RW}_${BS}
            echo "name="$NAME
            LD_LIBRARY_PATH=$LIBNBD_PATH $FIO --name=$NAME --direct=1 --ramp_time=$WARMUP_TIME --time_based=1 --runtime=$TIME --rw=$RW --direct=$DIRECT --loops=$LOOP --size=$SIZE --ioengine=nbd --iodepth=$IODEPTH --bs=$BS --uri=$URI
        done
    done
done
}

test_normal
test_nbd

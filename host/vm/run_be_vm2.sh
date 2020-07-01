#!/bin/bash

RUN_VM_SCRIPT=/home/gic4107/OmniVisor/host/vm/run_vm.sh

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] || [ -z $4 ]; then 
    echo "arg1 as enable dsag; arg2 as local memory size (MB); arg3 as # of cores; arg4 as memory size"
    exit
fi

DSAG_EN=$1
LOCAL_MEM=$2
CORES=$3
MEM=$4
DISK=/home/gic4107/OmniVisor/host/vm/BE_vm2.qcow2

$RUN_VM_SCRIPT $DSAG_EN $LOCAL_MEM $CORES $MEM $DISK "-net nic,model=virtio -net user,hostfwd=tcp::2223-:22,hostfwd=tcp::4443-:11211" 3
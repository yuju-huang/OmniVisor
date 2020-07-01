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
DISK=/home/gic4107/OmniVisor/host/vm/LC_vm1.qcow2

# Use tap network for better performance. Remember to setup tap0 first,
# ref /home/gic4107/OmniVisor/host/util_script/enable_vm_tap0.sh.
$RUN_VM_SCRIPT $DSAG_EN $LOCAL_MEM $CORES $MEM $DISK "-net nic,macaddr=DE:AD:BE:EF:90:26 -net tap,ifname=tap0,script=no" 1

#$RUN_VM_SCRIPT $DSAG_EN $LOCAL_MEM $CORES $MEM $DISK "-net nic,model=virtio -net user,hostfwd=tcp::2221-:22,hostfwd=tcp::4441-:11211" 1

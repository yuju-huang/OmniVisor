#!/bin/bash
sudo rmmod kvm_intel
sudo rmmod kvmgt
sudo rmmod kvm
sudo insmod /home/gic4107/linux/arch/x86/kvm/kvm.ko
sudo insmod /home/gic4107/linux/arch/x86/kvm/kvm-intel.ko
#sudo insmod /lib/modules/5.0.0+/kernel/arch/x86/kvm/kvm.ko 
#sudo insmod /lib/modules/5.0.0+/kernel/arch/x86/kvm/kvm-intel.ko 

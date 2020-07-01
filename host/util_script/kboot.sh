#!/bin/bash

[[ "$1" != '-' ]] && kernel="$1"
shift
if [[ "$1" == '-' ]]; then
    reuse=--reuse-cmdline
    shift
fi
[[ $# == 0 ]] && reuse=--reuse-cmdline
kernel="${kernel:-$(uname -r)}"
kargs="/boot/vmlinuz-$kernel --initrd=/boot/initrd.img-$kernel"

kexec -l -t bzImage $kargs $reuse --append="$*" && systemctl kexec

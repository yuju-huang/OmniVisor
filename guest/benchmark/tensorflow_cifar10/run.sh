#!/bin/bash

if [ -z $1 ]; then
    echo "arg1 as epoch"
    exit
fi
EPOCHS=$1

python3 tensorflow_cifar10.py $1

#!/bin/sh

MEMORY=$1
VERBOSE=$2

memcached -m $MEMORY $VERBOSE

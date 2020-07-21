#!/bin/bash

cat $1 | egrep -e "path|name|bw=" > tmp
vim -c '%s/MiB/ MiB/ | %s/KiB/ KiB/ | %s/ \+/ / | write | quit' tmp

awk -F '[= _]' '
BEGIN {
    testcase=""
    testsize=""
}
{
    if ($1 == "path") {
        print $2
    }
    else if ($1 == "name") {
        testcase=$2
        testsize=$3
    }
    else {
        print testcase","testsize","$4","$5
    }
}
END {

}' tmp

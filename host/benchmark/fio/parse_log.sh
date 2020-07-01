#!/bin/bash

cat $1 | egrep -e "path|name|BW" > tmp
vim -c '%s/MiB/ MiB/ | %s/KiB/ KiB/ | write | quit' tmp

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
        print testcase","testsize","$7","$8
    }
}
END {

}' tmp

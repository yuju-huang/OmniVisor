#!/bin/basdh

memaslap -s localhost:4441 -t 30s -F ./memslap_set_key1m_value4k.cnf -c 16 -T 8 -x 1000000
#memaslap -s localhost:4441 -t 10s -F ./memslap_get_key1m_value4k.cnf -c 16 -T 8

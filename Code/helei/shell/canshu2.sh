#!/bin/bash
for i in "$*"
    do
        echo $*
        echo "$i"
    done


for y in "$@"
    do
        echo $y
    done
echo $?

#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "usage ./runtest.sh <tool-executable> <test-dir>"; exit -1;
fi

if [ ! -f "$1" ]; then
    echo "$1" " doesn't exist";
elif [ ! -f "$2"]; then
    echo "$2" " doesn't exit";
fi

for file in $2/boolean/* $2/combined/* $2/fixpoints_only/* $2/modal_operators/*; do
    if [ ${file: -4} == ".mcf" ]; then
        $1 --algo=naive testcases/fixpoints_only/test.aut $file
    fi
done

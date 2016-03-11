#!/bin/sh

if ! [ -e $1 ]; then
    echo "usage ./runtest.sh <tool-executable> <test-dir>"; exit -1;
fi

for file in $2/**/*; do
    if [ ${file: -4} == ".mcf" ]; then
        $1 --algo=naive testcases/fixpoints_only/test.aut $file
    fi
done

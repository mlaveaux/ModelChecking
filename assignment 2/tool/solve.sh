#!/bin/sh

if [ -z $1 ] || [ -z $2 ]; then
    echo "Usage ./solve.sh <order> <wildcard_files>"
fi

for file in $2; do
    echo $file
    echo $file >> output.txt
    perf stat ./tool --order="$1" --output=winner "$file" >> output.txt;
done;

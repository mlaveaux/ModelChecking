#!/bin/sh

for file in testcases/**/*; do
	echo $file;
	tool/build/tool --algo=naive testcases/fixpoints_only/test.aut $file
done

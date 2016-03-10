#!/bin/sh

for file in testcases/**/*; do
	tool/build/tool --algo=naive testcases/fixpoints_only/test.aut $file
done

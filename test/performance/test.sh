#!/bin/sh

cd ../../circuits/

cmake --build ../build &>/dev/null

runs=10
function main(){
	for((i=0; i<$runs; i+=1)); do
		../build/main &>/dev/null
	done
}

TIMEFORMAT=%U

average=$(bc -l <<< "$((time $(main)) 2>&1)/$runs")

echo "Average time: $average"

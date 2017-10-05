#!/bin/bash

f="./results.txt"
run="./out/integrate.out"
args="1 10 100 1000 10000 100000 1000000"

echo ""
echo "Running script" $0
echo ""

echo "Results from running integration.out with different arguments." > ${f}

echo "" >> ${f}
for i in {0..4}
do
    numThreads=$((2 ** $i))
    echo "Running with" $numThreads "thread(s)" >> $f
    for numTrapezes in $args
    do
	out=$($run $numThreads $numTrapezes)
	echo $out
	echo "    $out" >> $f
    done
    echo "" >> $f
done

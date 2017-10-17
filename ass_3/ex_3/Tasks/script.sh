#!/bin/bash

r_file=results.txt
exes='./Merge_sortP ./Merge_sortS'

rm $r_file

echo "Running tests and saving the results in $r_file."

for exe in $exes
do
    echo "Running $exe"
    echo $exe >> $r_file
    for i in 1 2 4 8 16
    do
	echo "$exe $i"
	$exe $i >> $r_file
    done
done

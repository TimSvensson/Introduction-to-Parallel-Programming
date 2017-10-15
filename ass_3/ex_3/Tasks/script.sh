#!/bin/bash

r_file=results.txt
exe=./Merge_sortP

rm $r_file

echo "Running $exe and saving the results in $r_file."

for i in {1..3}
do
    echo "Running test $i"
    $exe >> $r_file
done

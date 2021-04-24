#!/bin/bash
# 
# File:   parse_files.bash
# Author: ginns
#
# Created on 21-Apr-2021, 12:14:06 AM
#


alg=$1

cd ..

if  [ "$alg" == A ]
then
    file_out="results_AVL.csv"
    file_prefix="data/AVL"
elif [ "$alg" == L ]
then
    file_out="results_LL.csv"
    file_prefix="data/LinkedList"
elif [ "$alg" == R ]
then
    file_out="results_RB.csv"
    file_prefix="data/RedBlack"
fi



rm -f "$file_out"

# create header
(cat "$file_prefix"/CATree*_1.txt | grep -E "^(totalThreads|keyRangeSize|insertPercent|deletePercent|millisToRun|completedOperations|throughput)=[A-Za-z0-9\.]+$" | cut -d"=" -f1 | tr "\n" "," | sed 's/.$//'; echo) >> "$file_out"

for file in "$file_prefix"/CATree*.txt; do
    ./bash_scripts/parse_file.bash "$file" >> "$file_out"
done

cd bash_scripts

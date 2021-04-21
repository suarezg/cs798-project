#!/bin/bash
# 
# File:   parse_files.bash
# Author: ginns
#
# Created on 21-Apr-2021, 12:14:06 AM
#


cd ..

file_out="results.csv"

rm -f "$file_out"

# create header
(cat data/CATree*_1.txt | grep -E "^(totalThreads|keyRangeSize|insertPercent|deletePercent|millisToRun|completedOperations|throughput)=[A-Za-z0-9\.]+$" | cut -d"=" -f1 | tr "\n" "," | sed 's/.$//'; echo) >> "$file_out"

for file in data/CATree*.txt; do
    ./bash_scripts/parse_file.bash "$file" >> "$file_out"
done

cd bash_scripts

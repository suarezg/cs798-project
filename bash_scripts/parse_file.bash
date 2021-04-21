#!/bin/bash
# 
# File:   parse_file.bash
# Author: ginns
#
# Created on 21-Apr-2021, 12:12:53 AM
#

datafile=$1

cat "$datafile" | grep -E "^(totalThreads|keyRangeSize|insertPercent|deletePercent|millisToRun|completedOperations|throughput)=[A-Za-z0-9\.]+$" | cut -d"=" -f2 | tr "\n" "," | sed 's/.$//'; echo



#cat CATree_1.txt | grep -E "^(totalThreads|keyRangeSize|insertPercent|deletePercent|millisToRun|completedOperations|throughput)=[A-Za-z0-9\.]+$" | cut -d"=" -f2 | tr "\n" "," | sed 's/.$//'; echo
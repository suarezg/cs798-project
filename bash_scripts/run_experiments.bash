#!/bin/bash
# 
# File:   run_experiments.bash
# Author: ginns
#
# Created on 20-Apr-2021, 11:17:51 PM
#

duration=$1
trials=$2


#allThreads=(2)
allThreads=(1 2 4 8 16)
keyRange=(100 10000)
# Workload
inPercent=(50 75 25 0)
delPercent=(50 25 75 0)

num=1

cd ..

if [ ! -d data ] 
then
    mkdir data
fi 


for thread in "${allThreads[@]}" ; do
    for range in "${keyRange[@]}"; do
        for ((j=0;j<"${#inPercent[@]}";j++)); do
            for ((i=1;i<=trials;i++)); do
                
              
                file_out="data/CATree_${num}.txt"
                cmd="./benchmark -t $duration -s $range -n $thread -i ${inPercent[j]} -d ${delPercent[j]} > $file_out"
   
                
                # delete file
                rm -f "$file_out"

                echo "Running experiment: [INS=${inPercent[j]}] [DEL=${delPercent[j]}] [KEYRANGE=$range] [THREAD=$thread] Trial $i"
                echo "$cmd"
                eval "$cmd"
                num=$((num+1))
            done
        done
    done
done

cd bash_scripts
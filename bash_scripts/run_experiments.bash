#!/bin/bash
# 
# File:   run_experiments.bash
# Author: ginns
#
# Created on 20-Apr-2021, 11:17:51 PM
#

duration=$1
trials=$2
alg=$3

#allThreads=(2)
allThreads=(1 2 4 8 16)
#keyRange=(100 10000)
keyRange=(10000)
# Workload
inPercent=(50 0)
delPercent=(50 0)

num=1

cd ..

if [ ! -d data ] 
then
    mkdir data
fi 

if [ ! -d data/AVL ]
then
    mkdir data/AVL
fi

if [ ! -d data/LinkedList ]
then
    mkdir data/LinkedList
fi

if [ ! -d data/RedBlack ]
then 
    mkdir data/RedBlack
fi


for thread in "${allThreads[@]}" ; do
    for range in "${keyRange[@]}"; do
        for ((j=0;j<"${#inPercent[@]}";j++)); do
            for ((i=1;i<=trials;i++)); do
                
                if [ -z "$alg" ] || [ "$alg" == A ]
                then
                    file_out="data/AVL/CATree_AVL_$num.txt"
                elif [ "$alg" == L ]
                then
                    file_out="data/LinkedList/CATree_LL_$num.txt"
                elif [ "$alg" == R ]
                then
                    file_out="data/RedBlack/CATree_RB_$num.txt"
                fi
                
                # delete file if it already exists
                rm -f "$file_out"

                cmd="taskset -c 0-7,16-23 ./benchmark -t $duration -s $range -n $thread -i ${inPercent[j]} -d ${delPercent[j]} -o $alg > $file_out"
                echo "Running experiment: [INS=${inPercent[j]}] [DEL=${delPercent[j]}] [KEYRANGE=$range] [THREAD=$thread] Trial $i"
                echo "$cmd"
                eval "$cmd"
                num=$((num+1))
            done
        done
    done
done

cd bash_scripts
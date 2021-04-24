#!/bin/bash
# 
# File:   check_validaion.bash
# Author: ginns
#
# Created on 20-Apr-2021, 11:54:56 PM
#

alg=$1

cd ..

if  [ "$alg" == A ]
then
    file_prefix="data/AVL"
elif [ "$alg" == L ]
then
    file_prefix="data/LinkedList"
elif [ "$alg" == R ]
then
    file_prefix="data/RedBlack"
fi


for file in "$file_prefix"/CATree*.txt; do
   cat "$file" | grep -E "^Validation: sum of keys according to the data structure = ([0-9]+) and sum of keys according to the threads = ([0-9]+). (OK|FAILED)." | cut -d "." -f2   
done

cd bash_scripts

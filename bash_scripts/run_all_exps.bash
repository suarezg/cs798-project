#!/bin/bash
# 
# File:   run_all_exps.bash.bash
# Author: ginns
#
# Created on 23-Apr-2021, 9:27:41 PM
#

duration=$1
trials=$2

echo "Running CA Tree with AVL Trees..."
./run_experiments.bash $duration $trials A

echo "Running CA Tree with LinkedList..."
./run_experiments.bash $duration $trials L

echo "Running CA Tree with RedBlack Trees..."
./run_experiments.bash $duration $trials R
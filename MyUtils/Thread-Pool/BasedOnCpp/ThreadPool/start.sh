#!/bin/bash

num=50
cmd="g++ utils.h thread_pool.h utils.cpp thread_pool.cpp main.cpp"

if [[ $# == 1 ]]; then
    num=$1
fi

if [[ $# == 2 ]]; then
    cmd+=" -DDEBUG_LOG=1"
fi

echo "" > log.txt
rm a.out

echo "编译命令：$cmd"
$cmd

./a.out $num | tee log.txt


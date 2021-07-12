#!/bin/sh

rm -rf stats

make

width=50
height=50

n_runs=100

for agent in random manhattan raw_hamilton dfs
do
    echo $agent
    for i in `seq $n_runs`
    do
        echo $agent $i
        ./smart-snako --width=$width --height=$height --agent=$agent
    done
done

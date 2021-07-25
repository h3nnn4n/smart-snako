#!/bin/sh

width=50
height=50

n_runs=10

for agent in random manhattan dfs ida_dfs raw_hamilton smart_snako
do
    echo $agent
    for i in `seq $n_runs`
    do
        echo $agent $i
        ./smart-snako --width=$width --height=$height --agent=$agent
    done
done

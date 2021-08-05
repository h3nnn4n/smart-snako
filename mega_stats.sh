#!/bin/sh

n_cores=`nproc --all`

mkdir -vp mega_stats

head -n1 runs/1/stats/log.csv > mega_stats/log.csv

for i in `seq $n_cores`
do
    cp -v runs/$i/stats/log.csv mega_stats/log_$i.csv
    tail -n+2 mega_stats/log_$i.csv >> mega_stats/log.csv
done

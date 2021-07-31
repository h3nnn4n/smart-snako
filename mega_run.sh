#!/bin/sh

rm -rv runs

make rebuild

n_cores=`nproc --all`

for i in `seq $n_cores`
do
    mkdir -vp runs/$i/

    cp -v *.sh runs/$i/
    cp -v smart-snako runs/$i/
done

for i in `seq $n_cores`
do
    cd runs/$i/
    sh loop_run.sh &
    cd ../../
done

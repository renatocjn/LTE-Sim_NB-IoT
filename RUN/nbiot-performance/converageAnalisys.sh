#!/bin/bash

ruList="1 3 6 12"
schedulers="rr pf mt"
nUE=75

for sched in $schedulers; do
echo -n "$sched "
for i in $ruList; do
	echo -n "$i `./bin/lte-sim-r5 OnlyNbIot 0.5 $nUE $sched $i $((RANDOM*RANDOM))|grep -i serv|cut -d\  -f2` "
done
echo
done

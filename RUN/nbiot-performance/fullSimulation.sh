#!/bin/bash

cd ~/Programas/lte-sim-r5

nChilds=0
nRuns=3

for traffic in nbiot; do
for scheduler in pf mt rr; do
#for nUe in 10 20 30 40 50 75 100 125 150 200 250 300 400 500 600; do
for nUe in 500 1500 2000; do
for r in $(seq $nRuns); do

	if [ $nChilds -lt 4 ]; then
		nChilds=$(($nChilds+1))
	else
		wait
		nChilds=0
	fi

	outDir="Executions/hugeTeste/traffic=$traffic/scheduler=$scheduler/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
	(time ./LTE-Sim SingleCellNbIot 2 $nUe $traffic $scheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/hugeTeste/traffic=$traffic/scheduler=$scheduler/"
done
./RUN/nbiot-performance/makeComparationsGraphs.py "Executions/hugeTeste/traffic=$traffic"
done

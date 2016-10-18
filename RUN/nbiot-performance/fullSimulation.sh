#!/bin/bash

cd ~/Programas/lte-sim-r5

nRuns=10

for scheduler in rr pf mt; do
for traffic in cbr video voip; do
for nUe in 10 20 30 40 50 75 100 125 150 175 200 225 250 275 300; do
#for nUe in 10 20 30; do
for r in $(seq $nRuns); do
	outDir="Executions/scheduler=$scheduler/traffic=$traffic/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
	(time ./LTE-Sim SingleCellNbIot 2 $nUe $traffic $scheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt
done
done
done
./RUN/nbiot-performance/makeGraphs.py "Executions/scheduler=$scheduler/traffic=$traffic/"
done

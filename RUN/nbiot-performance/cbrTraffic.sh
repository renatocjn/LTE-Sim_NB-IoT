#!/bin/bash

cd ~/Programas/lte-sim-r5

nRuns=5

for scheduler in rr mt; do
	for nUe in 10 20 30 40 50 75 100 125 150 175 200 225 250 275 300; do
		for r in $(seq $nRuns); do\
			outDir="Executions/Default/nUe=$nUe/$r/"
			mkdir -p "$outDir"

			#seed=$((RANDOM))

			(time ./LTE-Sim SingleCellNbIot 1 $nUe cbr $scheduler) > $outDir/traceLteSim.txt 2> $outDir/time.txt
		done
	done
done

./RUN/nbiot-performance/makeGraphs.py Executions/Default

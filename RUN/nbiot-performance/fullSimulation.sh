#!/bin/bash

cd ~/Programas/LTE-Sim_NBIot

nChilds=0
maxChilds=$(nproc)
nRuns=30
cellRadius=2 #2km

#Edit this for every execution
outFolder="newPF"


for traffic in m2m; do
echo traffic $traffic
for scheduler in pf; do
echo "    scheduler" $scheduler
#for nUe in 10 25 50 75 100 125 150 125 200 225 250 275 300; do
for nUe in 200 225 250 275 300; do
#for nUe in 10 50 200; do
for r in $(seq 4 $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/traffic=$traffic/scheduler=$scheduler/nUe=$nUe/$r/"
 	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	(time bin/LTE-Sim_NBIot SingleCellNbIot $cellRadius $nUe $traffic $scheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/traffic=$traffic/scheduler=$scheduler/"
done
./RUN/nbiot-performance/makeComparationsGraphs.py "Executions/$outFolder/traffic=$traffic"
done



for traffic in m2m; do
echo traffic $traffic
for scheduler in rr; do
echo "    scheduler" $scheduler
for nUe in 10 25 50 75 100 125 150 125 200 225 250 275 300; do
#for nUe in 225 250 275 300; do
#for nUe in 10 50 200; do
for r in $(seq 4 $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/traffic=$traffic/scheduler=$scheduler/nUe=$nUe/$r/"
 	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	(time bin/LTE-Sim_NBIot SingleCellNbIot $cellRadius $nUe $traffic $scheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/traffic=$traffic/scheduler=$scheduler/"
done
./RUN/nbiot-performance/makeComparationsGraphs.py "Executions/$outFolder/traffic=$traffic"
done

#!/bin/bash

if [ ! -x bin/LTE-Sim_NBIot ]; then
	echo You are not in the right directory
	exit 1
fi

nChilds=0
maxChilds=$(nproc)
nRuns=30

cellRadius=2 #2km
outFolder="testNBIoT"


for traffic in mixed; do
for scheduler in mt pf rr; do
for nUe in 10 25 50 75 100 125 150 125 200 225 250 275 300; do
#for nUe in 25 75 100; do
for r in $(seq $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/lteOnly/traffic=$traffic/scheduler=$scheduler/nUe=$nUe/$r/"
 	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	srun -p gpu bin/LTE-Sim_NBIot SingleCellM2mUnderLTE $cellRadius $nUe $traffic $scheduler $seed > $outDir/traceLteSim.txt&
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/lteOnly/traffic=$traffic/scheduler=$scheduler/"
done
#./RUN/nbiot-performance/makeLteComparationsGraphs.py "Executions/$outFolder/lteOnly/traffic=$traffic"
done


for scSpacing in 15; do
for scClusterSize in 1 3 6 12; do
for nUe in 10 25 50 75 100 125 150 125 200 225 250 275 300; do
#for nUe in 25 75 100; do
for r in $(seq $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/withNbIot/scClusterSize=$scClusterSize/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
	srun -p gpu bin/LTE-Sim_NBIot SingleCellM2mUnderNbIot $cellRadius $nUe $scSpacing $scClusterSize $seed > $outDir/traceLteSim.txt&
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/withNbIot/scClusterSize=$scClusterSize"
done
#./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/withNbIot/"
done

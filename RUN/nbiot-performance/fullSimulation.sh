#!/bin/bash

cd ~/Programas/LTE-Sim_NBIot

nChilds=0
maxChilds=$(nproc)
nRuns=3

cellRadius=2 #2km
outFolder="fullNbiot"


# for traffic in mixed; do
# for scheduler in mt pf rr; do
# # for nUe in 25 50 75 100 125 150 125 200 225 250 275 300; do
# for nUe in 25 50 75; do
# for r in $(seq $nRuns); do
#
#  	if [ $nChilds -lt $maxChilds ]; then
#  		nChilds=$(($nChilds+1))
#  	else
#  		wait
#  		nChilds=0
#  	fi
#
# 	outDir="Executions/$outFolder/lteOnly/traffic=$traffic/scheduler=$scheduler/nUe=$nUe/$r/"
#  	mkdir -p "$outDir"
#
# 	seed=$((RANDOM * RANDOM))
#  	#(time bin/LTE-Sim_NBIot SingleCellM2mUnderLTE $cellRadius $nUe $traffic $scheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
# done
# done
# wait
# nChilds=0
# ./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/lteOnly/traffic=$traffic/scheduler=$scheduler/"
# done
# ./RUN/nbiot-performance/makeLteComparationsGraphs.py "Executions/$outFolder/lteOnly/traffic=$traffic"
# done


for ulScheduler in pf mt rr; do
for scClusterSize in 1 3 6 12; do
#for nUe in 25 50 75 100 125 150 125 200 225 250 275 300; do
for nUe in 25 50 75; do
for r in $(seq $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/nbIotClusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	#(time bin/LTE-Sim_NBIot SingleCellM2mUnderNbIot $cellRadius $nUe $scSpacing $scClusterSize $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/nbIotClusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize"
done
./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/nbIotClusterSizeComparation/ulScheduler=$ulScheduler/"
done


for scClusterSize in 1 3 6 12; do
for ulScheduler in pf mt rr; do
#for nUe in 25 50 75 100 125 150 125 200 225 250 275 300; do
for nUe in 25 50 75; do
for r in $(seq $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/nbIotSchedComparation/ulSchedulerscClusterSize=$scClusterSize/ulscheduler=$ulScheduler/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	#(time bin/LTE-Sim_NBIot SingleCellM2mUnderNbIot $cellRadius $nUe $scSpacing $scClusterSize $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/nbIotSchedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler/"
done
./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/nbIotSchedComparation/scClusterSize=$scClusterSize/"

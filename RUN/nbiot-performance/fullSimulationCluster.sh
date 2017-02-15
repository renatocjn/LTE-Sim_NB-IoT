#!/bin/bash

if [ ! -x bin/LTE-Sim_NBIot ]; then
	echo You are not in the right directory
	exit 1
fi

nChilds=0
maxChilds=$(nproc)
nRuns=30

cellRadius=2 #2km
outFolder="quickFullNbiot"

nUeList="25 50 75 100 125 150 125 200 225 250 275 300"
#nUeList="25 50 75"
ulSchedulerList="pf mt rr"
scClusterSizeList="1 3 6 12"

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


for ulScheduler in $ulSchedulerList; do
for scClusterSize in $scClusterSizeList; do
for nUe in $nUeList; do
for r in $(seq $nRuns); do

 	if [ $nChilds -lt $maxChilds ]; then
 		nChilds=$(($nChilds+1))
 	else
 		wait
 		nChilds=0
 	fi

	outDir="Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	bin/LTE-Sim_NBIot SingleCellM2mUnderNbIot $cellRadius $nUe $ulScheduler $scClusterSize $seed > $outDir/traceLteSim.txt&
done
done
wait
nChilds=0
#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/nbIotClusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize"
done
#./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/"
done


for scClusterSize in $scClusterSizeList; do
for ulScheduler in $ulSchedulerList; do


mkdir -p "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"
cp -r "Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize" "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"

#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"
done
#./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize"
done

#!/bin/bash
#set -e

if [ ! -e "Executions" ]; then
	echo "You are in the wrong folder"
	exit 1
fi

nChilds=0
maxChilds=$(nproc)
nRuns=30

cellRadius="0.5" #500m
outFolder="underLTE"

nUeList="25 200 350 500 700 850 1000"
#nUeList="25 75 125 200"
ulSchedulerList="rr pf mt"
scClusterSizeList="1 3 6 12"

for r in $(seq $nRuns); do
for nUe in $nUeList; do
for ulScheduler in $ulSchedulerList; do

	if [ $nChilds -lt $maxChilds ]; then
		nChilds=$(($nChilds+1))
	else
		wait
		nChilds=0
	fi

	outDir="Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize/nUe=$nUe/$r/"
	mkdir -p "$outDir"
	
	if [ ! -e $outDir/traceLteSim.txt ]; then 
		seed=$((RANDOM * RANDOM))
	 	(time bin/lte-sim-r5 SingleCellM2mUnderLTE $cellRadius $nUe $ulScheduler $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
	fi
done
done
done
wait
nChilds=0

#./RUN/nbiot-performance/makeSuperNbIotGraphs.py "Executions/$outFolder/clusterSizeComparation"

#for sched in $ulSchedulerList; do
#	RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/clusterSizeComparation/ulScheduler=$sched"
#done 



#for scClusterSize in $scClusterSizeList; do
#echo "cluster size = $scClusterSize"
#for ulScheduler in $ulSchedulerList; do
#for nUe in $nUeList; do
#for r in $(seq $nRuns); do

#mkdir -p "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler/nUe=$nUe/$r"
#ln --symbolic --relative "Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize/nUe=$nUe/$r/traceLteSim.txt" "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler/nUe=$nUe/$r"

##mkdir -p "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"
##cp -r "Executions/$outFolder/clusterSizeComparation/ulScheduler=$ulScheduler/scClusterSize=$scClusterSize/*" "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"

#./RUN/nbiot-performance/makeScenarioGraphs.py "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize/ulscheduler=$ulScheduler"

#done
#done
#done
#RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/schedComparation/scClusterSize=$scClusterSize"
#done

cd "Executions/"
tar cfz "$outFolder.tar.gz" "$outFolder"

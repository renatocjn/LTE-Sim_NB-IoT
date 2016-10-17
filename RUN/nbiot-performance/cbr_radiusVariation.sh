#!/bin/bash

# min=10
# max=300
# step=$(($max - $min))
# step=$(($step/10))
nRuns=10

nCells=1
radius=1 #km
nUe=50

#Apps per UEs
nVoip=0
nVideo=0
mBE=0	# Infinite Buffer
nCBR=1

scheduler=1	#Proportional Fair
frameStructure=1	# FDD
speed=0 #stationary UEs
maxDelay=0.1
videoBitRate=128

seed=$((RANDOM))

for radius in 1 5 25 50 75 125 150; do
	for r in $(seq $nRuns); do
		outDir="Executions/radiusVariation/R=$radius/$r/"
		mkdir -p "$outDir"

		#seed=$((RANDOM))

		(time ./LTE-Sim SingleCellWithI $nCells $radius $nUe $nVoip $nVideo $mBE $nCBR $scheduler $frameStructure $speed $maxDelay $videoBitRate) > $outDir/traceLteSim.txt 2> $outDir/time.txt
	done
done

#./saveGraphs.py "Executions/radiusVariation" "Cell Radius (km)" "Variation in cell radius"

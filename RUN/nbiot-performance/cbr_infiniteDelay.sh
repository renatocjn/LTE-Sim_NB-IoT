#!/bin/bash

# min=10
# max=300
# step=$(($max - $min))
# step=$(($step/10))
nRuns=10

nCells=1
radius=1 #km

#Apps per UEs
nVoip=0
nVideo=0
mBE=0	# Infinite Buffer
nCBR=1

scheduler=1	#Proportional Fair
frameStructure=1	# FDD
speed=0 #non mobile UEs
maxDelay=9999
videoBitRate=128

seed=$((RANDOM))

for nUe in 10 50 75 100 125 150 175 200 225 250 275 300; do
	for r in $(seq $nRuns); do
		outDir="Executions/InfDelay/nUe=$nUe/$r/"
		mkdir -p "$outDir"

		#seed=$((RANDOM))

		(time ./LTE-Sim SingleCellWithI $nCells $radius $nUe $nVoip $nVideo $mBE $nCBR $scheduler $frameStructure $speed $maxDelay $videoBitRate) > $outDir/traceLteSim.txt 2> $outDir/time.txt
	done
done

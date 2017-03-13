#!/bin/bash
set -e

if [ ! -e "Executions" ]; then
	echo "You are in the wrong folder"
	exit 1
fi

nChilds=0
maxChilds=$(nproc)
nRuns=30

outFolder="nbiotEffectEvaluation"

nUeList="25 50 75 100 125 150 175 200 225 250 275 300"
#nUeList="25 50 75"

for bw in 3 10; do
if [ $bw == 3 ]; then nNbiotList="0 1 2"
else nNbiotList="0 3 6 8"
fi
for nNbiot in $nNbiotList; do
for nUe in $nUeList; do
for r in $(seq $nRuns); do

	if [ $nChilds -lt $maxChilds ]; then
		nChilds=$(($nChilds+1))
	else
		wait
		nChilds=0
	fi

	outDir="Executions/$outFolder/bandwidth=$bw/nNbiot=$nNbiot/nUe=$nUe/$r/"
	mkdir -p "$outDir"

	seed=$((RANDOM * RANDOM))
 	(time bin/LTE-Sim_NBIot NbIotEffectEvaluation $nUe $nNbiot $bw $seed) > $outDir/traceLteSim.txt 2> $outDir/time.txt &
done
done
wait
nChilds=0
done
./RUN/nbiot-performance/makeNbIotComparationsGraphs.py "Executions/$outFolder/bandwidth=$bw/"
done

cd "Executions/"
tar cfz "$outFolder.tar.gz" "$outFolder"

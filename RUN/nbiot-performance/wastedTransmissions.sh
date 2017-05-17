for conf in 12; do 
for sched in rr pf mt; do 
echo -n "$conf $sched " 

for i in `seq 20`; do 
	echo -n "`bin/lte-sim-r5 OnlyNbIot 0.5 75 pf 3 $((RANDOM*RANDOM))|grep DROP|cut -d\  -f 10|awk '{sum+=$1} END {print sum;}'` "
done 
echo 

done 
done



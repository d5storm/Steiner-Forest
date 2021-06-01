#!/bin/bash

function EXEC {
	$*
	# if [ $? -ne 0 ]
	# then
	# 	#echo "Processo Interrompido"
	# 	#exit 1
	# fi
}

function CTRLC {
	echo "CTRL C ---------------------------------- PRESSIONADO!!!"
	exit 1 
}


execpath="./bin/heuristic"
#instances="./Instances/sergio/*"
instances="./Instances/testing/*"
declare -a support=(3)
declare -a gamma=(12)
declare -a eliteSetSize=(10)
declare -a alphas=(1)
declare -a execTime=( "7.59" "14.58" "517.51" "1090.38" "2858.66" "4.53" "1185.03" "4065.35" "5.58" "9.74" "0.05" "0.28" "0.35" "845.94" )



trap CTRLC SIGINT

timePos=0

for file in $instances
do
	echo $file
	for ((  s = 1 ;  s <= 10;  s++  )) 
	do	
		for g in "${gamma[@]}"
		do
			# value=${execTime[timePos]}
			# echo $value
			EXEC $execpath $file $s 1 200 10 3 -1 true true $g  # >> ./results/new_instances_dm.txt 
		done
	done	
((timePos=timePos+1))
done




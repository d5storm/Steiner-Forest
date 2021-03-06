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
instances="./Instances/sergio/*"
declare -a uchoaIter=(1)
declare -a algIter=(200)
declare -a perturbation=(5)
declare -a alphas=(4 3 2 1)
declare -a execTime=("0.4068" "0.7283" "6.0801" "9.1492" "18.5216" "0.4252" "0.9441" "8.3798" "11.7800" "24.6800" "0.8983" "0.0061" "0.0117" "0.0289" "0.8640" "0.6594" "1.5197" "0.0435" "0.1620" "0.2041" "18.2593" "34.0608")



trap CTRLC SIGINT

timePos=0
for a in "${alphas[@]}"
do
	for file in $instances
	do
	# for p in "${perturbation[@]}"
	# do
	# 	for u in "${uchoaIter[@]}"
	# 	do
	
		for ((  s = 1 ;  s <= 10;  s++  )) 
		do	

			for i in "${algIter[@]}"
			do
				value=${execTime[timePos]}
				# echo $value
				EXEC $execpath $file $s $a $i 0 0 -1 >> ./results/luidi/grasp_$i\_$a.txt
			done
		done
	done		
	# 	done
	# done
	# ((timePos=timePos+1))
done



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
# instances="./Instances/sergio/*"
instances="./Instances/testing/*"
declare -a support=(4)
declare -a algIter=(200)
declare -a eliteSetSize=(20)
declare -a alphas=(1)
declare -a execTime=("7.58" "14.54" "4.52" "6.73" "479.40" "843.29")



trap CTRLC SIGINT

timePos=0
for a in "${alphas[@]}"
do
	for file in $instances
	do
		echo $file
		for p in "${eliteSetSize[@]}"
		do
			for u in "${support[@]}"
			do

				for ((  s = 1 ;  s <= 10;  s++  )) 
				do	

					for i in "${algIter[@]}"
					do
						value=${execTime[timePos]}
						# echo $value
						EXEC $execpath $file $s 1 $i $p $u $value false >> ./results/dm_vs_regular_time/regular.txt 
					done
				done
			done		
		done
	((timePos=timePos+1))
	done
done



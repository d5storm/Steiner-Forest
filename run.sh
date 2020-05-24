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
instances="./Instances/"
dat=".dat"
# declare -a wkf=("CyberShake_30.xml" "CyberShake_50.xml")
# declare -a times=("3" "3")
# declare -a wkf=("GENOME.d.702049732.0.dax" "Montage_25.xml" "Montage_50.xml" "Montage_100.xml" "Inspiral_30.xml" "Inspiral_50.xml" "Inspiral_100.xml")
# declare -a times=("2245" "2665" "2746" "3355" "3382" "3490" "4021")
declare -a forest=("jmp-n-25-a-160-p-025-k-2-s-4711" "jmp-n-50-a-160-p-025-k-5-s-4731" "jmp-n-50-a-200-p-025-k-5-s-4781" "jmp-n-100-a-200-p-025-k-5-s-4786"
                   "jmp-n-100-a-200-p-025-k-10-s-4791" "jmp-n-100-a-200-p-050-k-15-s-4911" "jmp-n-500-a-200-p-025-k-35-s-5192" "jmp-n-500-a-200-p-050-k-50-s-5226")
declare -a uchoaIter=(100)
trap CTRLC SIGINT

## now loop through the above array
for i in "${uchoaIter[@]}"
do
	for file in "${forest[@]}"
	do
		for ((  s = 1 ;  s <= 10;  s++  )) 
		do	
			EXEC $execpath $instances$file$dat $s $i
		done
		time=$((time+1))			
	done
done


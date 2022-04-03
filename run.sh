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
instances="./Instances/novas_ghalami/teste_viabilidade/*"
declare -a support=(3)
declare -a gamma=(12)
declare -a eliteSetSize=(10)
declare -a alphas=(1)
declare -a execTime=("0.0003182" "0.0003674" "0.0003685" "0.0004222" "0.000386" "0.0004516" "0.0005491" "0.0005205" "0.0006266" "0.0007115" "0.0006733" "0.0007514" "0.0007631" "0.0008548" "0.0009588" "0.0012632" "0.000982" "0.0010026" "0.0045708" "0.0036991" "0.0052247" "0.0070714" "0.0077273" "0.0161596" "0.0125003" "0.0176823" "0.0192643" "0.0254858" "0.0114452" "0.0089872" "0.0182015" "0.0223314")
declare -a target=("94.00" "84.00" "144.00" "61.00" "61.00" "127.00" "112.00" "108.00" "223.00" "96.00" "93.00" "174.00" "178.00" "244.00" "320.00" "138.00" "134.00" "224.00" "144.00" "74.00" "114.00" "33.00" "47.00" "132.00" "237.00" "1623.00" "2011.00" "3301.00" "89.00" "105.00" "1153.00" "1550.00")




trap CTRLC SIGINT

timePos=0
targetPos=0
for file in $instances
do
	# echo $file
	for ((  s = 1 ;  s <= 1;  s++  )) 
	do	
		for g in "${gamma[@]}"
		do
			target=${target[targetPos]}
			time=${execTime[timePos]}
			EXEC $execpath $file $s 1 200 10 3 -1 false false $g 0 #>> ./results/ghalami/ttt_plot.txt 
		done
	done	
((timePos=timePos+1))
((targetPos=targetPos+1))
done




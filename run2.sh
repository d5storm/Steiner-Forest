#!/bin/bash

EXEC () {
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
instances="./Instances/plots_mining_effects/*"
# instances="./Instances/SteinForestLibrary/solved/*"
# instances="./Instances/time_plots/instances/*"
instances="./Instances/hard_instances/*"
declare -a support=(2)
declare -a gamma=(13)
declare -a eliteSetSize=(10)
declare -a alphas=(1)
declare -a execTime=("0.0003182" "0.0003674" "0.0003685" "0.0004222" "0.000386" "0.0004516" "0.0005491" "0.0005205" "0.0006266" "0.0007115" "0.0006733" "0.0007514" "0.0007631" "0.0008548" "0.0009588" "0.0012632" "0.000982" "0.0010026" "0.0045708" "0.0036991" "0.0052247" "0.0070714" "0.0077273" "0.0161596" "0.0125003" "0.0176823" "0.0192643" "0.0254858" "0.0114452" "0.0089872" "0.0182015" "0.0223314")
declare -a target=("94.00" "84.00" "144.00" "61.00" "61.00" "127.00" "112.00" "108.00" "223.00" "96.00" "93.00" "174.00" "178.00" "244.00" "320.00" "138.00" "134.00" "224.00" "144.00" "74.00" "114.00" "33.00" "47.00" "132.00" "237.00" "1623.00" "2011.00" "3301.00" "89.00" "105.00" "1153.00" "1550.00")

declare -a execTime=("0.08131446" "0.1103473" "0.09755925" "0.3204651" "0.1804684" "0.253666" "0.232158" "0.7579853" "0.188995" "0.1808633" "0.1996497" "0.1896434" "0.9359699" "0.7536402" "0.3093236" "0.2412764" "0.5055419" "0.8500098" "0.2326161" "0.2692661" "0.2785271" "0.2419921" "0.4136296" "0.3005053" "0.3939209" "0.6290151" "0.5171669" "0.5544257" "0.554828" "6.12026" "1.594935" "1.474871" "1.346934" "1.456639" "1.212759" "5.84227" "1.057646" "1.056094" "1.05549" "1.025677" "1.089105" "14.05492" "3.886207" "5.542809" "2.87364" "2.239657" "2.981047" "1.093765" "1.094137" "1.095906" "10.85449" "1.75763" "1.787809" "1.829301" "1.872495" "1.748928" "1.547668" "1.547256" "1.440943" "2.647853" "1.956044" "1.28008" "4.709986" "6.961154" "3.00858" "3.015059" "3.398745" "7.772038" "3.205436" "3.208227" "48.22949" "5.967912" "10.52889" "8.35524" "11.34061" "13.88218" "3.804601" "3.795017" "3.794628" "3.797541" "3.791954" "46.44778" "42.79212" "5.559293" "5.829912" "5.696604" "5.690504" "5.160728" "186.7769" "68.71864" "90.70753" "85.22892" "72.34835" "46.83853" "8.29879" "8.304344" "8.297849" "8.320063" "8.305821" "256.453" "212.4802" "231.9685" "243.4791" "14.35975" "13.91574" "13.0336" "13.62199" "13.60252" "9.024863" "5.441541" "5.446181" "7.032779" "7.017587" "7.009305" "98.70448" "18.11617" "26.21907" "23.20927" "22.21613" "20.08769" "12.89767" "12.83807" "12.88879" "12.86236" "12.91285" "92.29399" "90.6557" "16.46209" "17.97551" "17.46456" "16.47472" "16.98693" "406.0279" "515.4749" "64.74868" "42.73234" "50.99174" "66.29047" "77.1576" "23.6637" "23.67867" "23.69135" "23.65254" "23.6801" "367.7017" "369.9666" "475.7965" "280.3664" "34.57319" "33.99592" "34.06261" "33" "34.0294" "2298.295" "2531.83" "2551.44" "2606.609" "2960.427" "70.81738" "70.5179" "70.52335" "70.53385" "70.52593" "3500.289" "3261.885" "3486.115" "3549.035" "3611.655" "118.6315" "118.8931" "121.2377" "118.7531" "122.9392")
declare -a dm_grasp_target=("1194" "1777" "1479" "3353" "3076" "3258" "3477" "4339" "2971" "2883" "3065" "2959" "5643" "4316" "4250" "4044" "4983" "5016" "3667" "4005" "4108" "3759" "1864" "1188" "1463" "2848" "2482" "2244" "2265" "6923" "5305" "5489" "5300" "5524" "5328" "6451" "4428" "4438" "4433" "4307" "4532" "11009" "8869" "8768" "8552" "8388" "8959" "6009" "6021" "6048" "10633" "7397" "7485" "7554" "7637" "7356" "1854" "1858" "1781" "2577" "2379" "1372" "4241" "3674" "3742" "3758" "4138" "5573" "3278" "3303" "10479" "7582" "7746" "7176" "7662" "7937" "5119" "5103" "5050" "5045" "5099" "9698" "9640" "6169" "6327" "6238" "6217" "5878" "22574" "18110" "17960" "17960" "18210" "17843" "12037" "11991" "12004" "11949" "11959" "21724" "21385" "21002" "21265" "15328" "15131" "14559" "14895" "14872" "2252" "1376" "1412" "1752" "1748" "1749" "9337" "5672" "6289" "6016" "5846" "6214" "3794" "3789" "3774" "3809" "3770" "8110" "7893" "4472" "4724" "4659" "4446" "4573" "15876" "16576" "11847" "11386" "11857" "12051" "12262" "7531" "7473" "7474" "7474" "7480" "14840" "14521" "14887" "14578" "9359" "9300" "9275" "9138" "9294" "36455" "36260" "35785" "35870" "35648" "23926" "23961" "24064" "23941" "24028" "42901" "42763" "42507" "42926" "43123" "29211" "29287" "29700" "29324" "29543" )
declare -a grasp_target=("1194" "1777" "1479" "3353" "3076" "3258" "3477" "4339" "2971" "2883" "3065" "2959" "5643" "4316" "4250" "4044" "4983" "5016" "3667" "4005" "4108" "3759" "1864" "1188" "1463" "2848" "2482" "2244" "2265" "6923" "5305" "5489" "5300" "5524" "5328" "6535" "4428" "4438" "4433" "4307" "4532" "11009" "8869" "8768" "8552" "8388" "8959" "6009" "6021" "6048" "10633" "7397" "7485" "7554" "7637" "7356" "1854" "1858" "1781" "2577" "2379" "1372" "4241" "3674" "3742" "3758" "4138" "5573" "3278" "3303" "10479" "7582" "7746" "7176" "7662" "7937" "5119" "5103" "5050" "5045" "5099" "9698" "9640" "6169" "6327" "6238" "6217" "5878" "22551" "18144" "17976" "18040" "18210" "17843" "12037" "11991" "12004" "11949" "11959" "21745" "21385" "21002" "21264" "15328" "15131" "14559" "14895" "14872" "2252" "1376" "1412" "1752" "1748" "1749" "9337" "5672" "6289" "6016" "5846" "6214" "3794" "3789" "3774" "3809" "3770" "8110" "7898" "4472" "4724" "4659" "4446" "4573" "15876" "16576" "11847" "11386" "11857" "12051" "12262" "7531" "7473" "7474" "7474" "7480" "14840" "14521" "14914" "14578" "9359" "9300" "9275" "9138" "9294" "36568" "36452" "35934" "36061" "35751" "23926" "23961" "24064" "23941" "24028" "43139" "43115" "42769" "43200" "43355" "29211" "29287" "29700" "29324" "29543")
declare -a grasp_target=("29211" "29287" "29700" "29324" "29543")
# declare filename



trap CTRLC SIGINT

timePos=0
targetPos=0
for file in $instances
do
	# IFS="/"
	readarray -d / -t split <<< $file
	filename=${split[3]}
	readarray -d . -t namesplit <<< ${filename}
	name=${namesplit[0]}
	# IFS="."
	# readarray -d . -t split <<< $name
	# cleanname=${split[0]}
	echo $file
	for ((  s = 5 ;  s <= 10;  s++  ))
	do
		for g in "${gamma[@]}"
		do
			# target=${grasp_target[targetPos]}
			# time=${execTime[timePos]}
			# echo ./results/pattern_extraction/${name}_${s}.pattern
			EXEC $execpath $file $s -1 -1 10 2 1200 true true $g -1 > ./results/pattern_extraction_1200s/${name}_${s}.pattern
			# EXEC $execpath $file $s -1 200 10 2 -1 false false $g -1 >> ./results/plots_mining_effects/$filename
		done
	done
((timePos=timePos+1))
((targetPos=targetPos+1))
done




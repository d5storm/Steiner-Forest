set xlabel 'Iteration'
set ylabel 'Time'
set xrange [0:200]
set yrange [0:2]
set key outside bottom
set terminal pdf
set output 'i640-105_bl.pdf'
set arrow from 82, graph 0 to 82, graph 1 nohead
plot 'dm_i640-105_sft.stp' u 0:4 w p ls 1 t "DM-GRASP-SFP-Local Search", 'i640-105_sft.stp' u 0:4 w p ls 3 t "GRASP-SFP-Local Search"

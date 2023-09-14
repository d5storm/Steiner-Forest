set xlabel 'Iteration'
set ylabel 'Time'
set xrange [0:200]
set yrange [0:5]
set key outside bottom
set terminal pdf
set output 'i640-231_bl.pdf'
set arrow from 64, graph 0 to 64, graph 1 nohead
set arrow from 124, graph 0 to 124, graph 1 nohead
plot 'dm_i640-231_sft.stp' u 0:4 w p ls 1 t "DM-GRASP-SFP-Local Search", 'i640-231_sft.stp' u 0:4 w p ls 3 t "GRASP-SFP-Local Search"

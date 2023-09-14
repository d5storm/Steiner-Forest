set xlabel 'Iteration'
set ylabel 'Time'
set xrange [0:200]
set yrange [0:0.1]
set key outside bottom
set terminal pdf
set output 'i640-235_cons.pdf'
set arrow from 89, graph 0 to 89, graph 1 nohead
set arrow from 169, graph 0 to 169, graph 1 nohead
plot 'dm_i640-235_sft.stp' u 0:3 w p ls 1 t "DM-GRASP-SFP-Constructor", 'i640-235_sft.stp' u 0:3 w p ls 3 t "GRASP-SFP-Constructor"

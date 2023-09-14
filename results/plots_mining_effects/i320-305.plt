set xlabel 'Iteration'
set ylabel 'Time'
set xrange [0:200]
set yrange [0:0.05]
set key outside bottom
set terminal pdf
set output 'i320-305_cons.pdf'
set arrow from 53, graph 0 to 53, graph 1 nohead
set arrow from 89, graph 0 to 89, graph 1 nohead
set arrow from 180, graph 0 to 180, graph 1 nohead
plot 'dm_i320-305_sft.stp' u 0:3 w p ls 1 t "DM-GRASP-SFPConstructor", 'i320-305_sft.stp' u 0:3 w p ls 3 t "GRASP-SFP-Constructor"

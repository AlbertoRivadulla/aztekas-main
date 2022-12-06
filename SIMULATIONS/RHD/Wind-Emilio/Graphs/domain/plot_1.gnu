#!/usr/bin/gnuplot

set terminal epslatex
set output "plot1.tex"

set title "$a = 0.99$, MC, $v_i$ free"
set xlabel "$t/M$"
#set xrange [650:35000]
set ylabel "$\\dot{M}$"
set yrange [4.85:5.1]

plot \
"mdot-5rs-mc.dat" u 1:2 w l lw 2 t "$R_\\mathrm{max} = \\phantom{2}5 \\, r_s$", \
"mdot-10rs-mc.dat" u 1:2 w l lw 2 t "$R_\\mathrm{max} = 10 \\, r_s$", \
"mdot-20rs-mc.dat" u 1:2 w l lw 2 t "$R_\\mathrm{max} = 20 \\, r_s$"

set out

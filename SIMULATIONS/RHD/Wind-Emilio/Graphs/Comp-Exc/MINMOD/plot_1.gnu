#!/usr/bin/gnuplot

set terminal epslatex
set output "plot1.tex"

set title "Excised boundaries MINMOD"
set xlabel "$r/M$"
set ylabel "$v_r$"

plot \
"no-exc.dat" u 1:($5 + 0.0) w l t "No excision", \
"exc.dat" u 1:($5 + 0.2) w l t "Excision"

set out

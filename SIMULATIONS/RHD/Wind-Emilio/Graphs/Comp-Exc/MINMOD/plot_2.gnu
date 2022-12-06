#!/usr/bin/gnuplot

set terminal epslatex
set output "plot2.tex"

set title "Excised boundaries MINMOD"
set xlabel "$t/M$"
set ylabel "$\\left| 1 - \\dot{M}_\\mathrm{min}/\\dot{M}_\\mathrm{max} \\right|$"

set logscale y

plot \
"mdot-no-exc.dat" u 1:5 w l t "No excision", \
"mdot-exc.dat" u 1:5 w l t "Excision"

set out

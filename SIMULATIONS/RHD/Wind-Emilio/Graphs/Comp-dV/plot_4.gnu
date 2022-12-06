#!/usr/bin/gnuplot

set terminal epslatex
set output "plot4.tex"

set title "Comparisson different $\\Delta V$
set xlabel "$t/M$"
set ylabel "$\\left| 1 - \\dot{M}_\\mathrm{min}/\\dot{M}_\\mathrm{max} \\right|$"

set logscale y

plot \
"mdot-mm-old-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ at the center of $x_{i,j,k}$", \
"mdot-mm-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ integrated at $x_{i,j,k}$", \
"mdot-mm-new-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ integrated along specific direction"

set out

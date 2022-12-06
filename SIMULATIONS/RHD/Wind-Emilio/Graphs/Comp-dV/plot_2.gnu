#!/usr/bin/gnuplot

set terminal epslatex
set output "plot2.tex"

set title "Comparisson different $\\Delta V$
set xlabel "$t/M$"
set ylabel "$\\dot{M}$"

set logscale y

plot \
"mdot-mc-old-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ at the center of $x_{i,j,k}$", \
"mdot-mc-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ integrated at $x_{i,j,k}$", \
"mdot-mc-new-dV.dat" u 1:5 w l t "$\\sqrt{\\gamma}$ integrated along specific direction"

set out

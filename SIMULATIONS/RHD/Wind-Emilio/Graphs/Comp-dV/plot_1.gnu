#!/usr/bin/gnuplot

set terminal epslatex
set output "plot1.tex"

set title "Comparisson different $\\Delta V$
set xlabel "$r/M$"
set ylabel "$v_r$"

plot \
"mc-old-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ at the center of $x_{i,j,k}$", \
"mc-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ integrated at $x_{i,j,k}$", \
"mc-new-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ integrated along specific direction"

set out

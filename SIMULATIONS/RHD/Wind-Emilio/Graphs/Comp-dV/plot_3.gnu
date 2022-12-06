#!/usr/bin/gnuplot

set terminal epslatex
set output "plot3.tex"

set title "Comparisson different $\\Delta V$
set xlabel "$r/M$"
set ylabel "$v_r$"

plot \
"mm-old-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ at the center of $x_{i,j,k}$", \
"mm-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ integrated at $x_{i,j,k}$", \
"mm-new-dV.dat" u 1:5 w lp t "$\\sqrt{\\gamma}$ integrated along specific direction"

set out

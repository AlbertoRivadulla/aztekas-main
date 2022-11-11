#!/bin/bash

# This script will run several simulations automatically, one after the other.
# They all have fixed
#   K = 4/3, Mach = 5
# The ECG coupling epsilon and the velocity at infinity will change. They take the values:
#   epsilon = 5, 20, 500
#   v_inf   = 0.2, 0.5, 0.9
# The aztekas code must be compiled for the different values of v_inf desired

# Instead of doing a loop for v_inf, I write the different values by hand.
# This allows me to control better which simulations are run each time.

progress_file="simulation_progress.log"
log_file="log_file.log"
paramfile_base="./paramfiles_ECG/wind_ECG_"
aztekas_bin_base="./aztekas_binaries/aztekas_vinf_"

# v_inf = 0.2 
v_inf=0_2
for epsilon_ECG in 5 20 500
do
    # Get the path of the binary and the parameter files
    aztekas_bin="${aztekas_bin_base}${v_inf}"
    paramfile="${paramfile_base}${epsilon_ECG}_vinf_${v_inf}.param"

    # Run the simulation
    ./$aztekas_bin $paramfile
    # Redirect stdout and stderr to the log file
    # ./$aztekas_bin $paramfile >$log_file 2>&1
    # Print stdout to the terminal, and redirect both stdout and stderr to the log file
    # ./$aztekas_bin $paramfile 2>&1 | tee $log_file

    # Write to the log file
    echo "Finished running the simulation with epsilon = ${epsilon_ECG} and v_inf = ${v_inf}." >> $progress_file
done
echo "" >> $log_file

# v_inf = 0.5
v_inf=0_5
for epsilon_ECG in 5 20 500
do
    # Get the path of the binary and the parameter files
    aztekas_bin="${aztekas_bin_base}${v_inf}"
    paramfile="${paramfile_base}${epsilon_ECG}_vinf_${v_inf}.param"

    # Run the simulation
    ./$aztekas_bin $paramfile
    # Redirect stdout and stderr to the log file
    # ./$aztekas_bin $paramfile >$log_file 2>&1
    # Print stdout to the terminal, and redirect both stdout and stderr to the log file
    # ./$aztekas_bin $paramfile 2>&1 | tee $log_file

    # Write to the log file
    echo "Finished running the simulation with epsilon = ${epsilon_ECG} and v_inf = ${v_inf}." >> $progress_file
done
echo "" >> $log_file


# v_inf = 0.9
v_inf=0_5
for epsilon_ECG in 5 20 500
do
    # Get the path of the binary and the parameter files
    aztekas_bin="${aztekas_bin_base}${v_inf}"
    paramfile="${paramfile_base}${epsilon_ECG}_vinf_${v_inf}.param"

    # Run the simulation
    ./$aztekas_bin $paramfile
    # Redirect stdout and stderr to the log file
    # ./$aztekas_bin $paramfile >$log_file 2>&1
    # Print stdout to the terminal, and redirect both stdout and stderr to the log file
    # ./$aztekas_bin $paramfile 2>&1 | tee $log_file

    # Write to the log file
    echo "Finished running the simulation with epsilon = ${epsilon_ECG} and v_inf = ${v_inf}." >> $progress_file
done
echo "" >> $log_file

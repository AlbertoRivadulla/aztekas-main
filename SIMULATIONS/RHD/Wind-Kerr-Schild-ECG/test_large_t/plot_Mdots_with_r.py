import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import re

if len(sys.argv) == 1:
    print("Run the script as:")
    print("\t python plot_several_Mdots.py v_inf [save_results: 0 or 1] [results_directory]")

# Value of v_inf
v_inf = 0.5
if len(sys.argv) > 1:
    v_inf = float(sys.argv[1])
v_inf_str = str( v_inf ).replace( '.', '_' )

# Value of the constant rho_0
rho0 = 10**(-10)

# Proportion of values used to compute the average at the end
prop_average = 0.1
# prop_average = 0.05
# Boolean variable that determines whether or not saving the results
save_results = False
if len(sys.argv) > 2:
    if int(sys.argv[2]) == 1:
        save_results = True

# Base directory of the results
# base_directory_results = './tests_Mdot_with_r/results_ECG/K_4over3'
base_directory_results = './results_ECG/K_4over3'
if len(sys.argv) > 3:
    base_directory_results = sys.argv[3]
if base_directory_results[-1] != '/':
    base_directory_results += '/'
directory_results = base_directory_results + 'vinf_' + v_inf_str + '/'

# Go over the subfolders to get the different values of epsilon
subdirs = os.listdir( directory_results )
# Get the values of epsilon from this
eps_vals = []
for subdir in subdirs:
    match = re.findall( r"eps_([0-9]+)_cou_10$", subdir )
    if len( match ) > 0:
        eps_vals.append( int( match[0] ) )
eps_vals.sort()

# Create the figure
# fig, ax = plt.subplots( len(eps_vals), figsize=(7, 7) )
fig = plt.figure( figsize=(6, 4) )

# Plot the data
values_Mdot_MdotBHL   = [ 0. ] * len(eps_vals)
# values_log_Mdot_Mdot0 = [ 0. ] * len(eps_vals)
last_t_val            = [ 0. ] * len(eps_vals)
colors = plt.cm.rainbow( np.linspace(0, 1, len(eps_vals)) )
for i in range( len( eps_vals ) ):
    try:
        # Get the directory of the data
        directory_data = directory_results + 'eps_' + str( eps_vals[i] ) + "_cou_10" + "/Analysis/"
        # Get the data
        files = os.listdir( directory_data )
        filecount = 0
        for file in files:
            match = re.findall( r"Value_vs_r_([0-9]+)\.dat", file )
            if len(match) > 0:
                filecount = max( filecount, int(match[0]) )
        filepath = directory_data + "Value_vs_r_{}.dat".format(filecount)
        dataMdot = np.loadtxt( filepath , skiprows=5 )

        # Compute the values of r / r_max
        values_r_over_rmax = [ 0 ] * len( dataMdot )
        for j in range( len( dataMdot ) ):
            values_r_over_rmax[ j ] = dataMdot[ j, 0 ] / dataMdot[ -1, 0 ]

        # Compute the constants to normalize the data
        MdotBHL = 4 * np.pi * rho0 / (v_inf**3)
        Mdot0 = 4 * np.pi * rho0

        # Get the value of time at this point
        with open( filepath, "r") as f:
            f.readline()
            last_t_val[ i ] = float( f.readline() )

        # Compute the average of the values in the entire domain
        value_Mdot_avg = sum( dataMdot[:, 1] ) / len( dataMdot )
        values_Mdot_MdotBHL[ i ]   = value_Mdot_avg / MdotBHL

        # Plot the data
        plt.plot( values_r_over_rmax, dataMdot[:, 1] / MdotBHL,
                  label="eps = {}, t = {}".format( eps_vals[i], round(last_t_val[ i ]) ),
                  c=colors[i]
                )

        # Plot also a line with the average
        plt.plot( ( 0., 1. ),
                  ( values_Mdot_MdotBHL[i], values_Mdot_MdotBHL[i] ),
                  '-.',
                  # c=colors[i]
                  c="grey"
                )
    except:
        continue

# Print the values of Mdot computed
for i in range( len( eps_vals ) ):
    print("epsilon = {}, t_final = {}".format( eps_vals[i], last_t_val[ i ] ) )
    print("\t{:40s}{}".format("Average Mdot / Mdot_BHL", values_Mdot_MdotBHL[ i ]) )
    # print("\t{:40s}{}".format("Average log(Mdot / Mdot_0)", values_log_Mdot_Mdot0[ i ]) )

plt.xlabel( "r/r_max" )
plt.ylabel( "Mdot/Mdot_BHL" )
plt.legend( loc="best" )

# Save the results to a file
if save_results:
    print("Saving the average values to a file")
    # Save also the plot
    plot_dir = "./results_ECG/K_4over3/results_tests/Mdot_with_r_vinf_" + v_inf_str + ".png"
    plt.savefig( plot_dir, dpi=250 )

plt.show()

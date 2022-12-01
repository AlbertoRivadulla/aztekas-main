import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import re

if len(sys.argv) == 1:
    print("Run the script as:")
    print("\t python plot_several_Mdots.py v_inf [save_results: 0 or 1] [results_directory]")

# Value of v_inf
v_inf = 0.6
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
base_directory_results = './results_ECG/K_4over3'
if len(sys.argv) > 3:
    base_directory_results = sys.argv[3]
if base_directory_results[-1] != '/':
    base_directory_results += '/'
directory_results = base_directory_results + 'vinf_' + v_inf_str + '/'

# Go over the subfolders to get the different values of epsilon
subdirs = os.listdir( directory_results )
# Get the values of epsilon from this
eps_N_vals = []
for subdir in subdirs:
    match = re.findall( r"eps_([0-9]+)_rmin_(\d*\.?\d+)_racc$", subdir )
    if len( match ) > 0:
        if len( match[0] ) > 0:
            eps_N_vals.append( [ int( match[0][0] ), float( match[0][1] ) ] )
eps_N_vals.sort( key=lambda x: float(x[1]) )

# Create the figure
# fig, ax = plt.subplots( len(eps_vals), figsize=(7, 7) )
fig = plt.figure( figsize=(6, 4) )

# Plot the data
values_Mdot_MdotBHL   = [ 0. ] * len(eps_N_vals)
values_log_Mdot_Mdot0 = [ 0. ] * len(eps_N_vals)
last_t_val            = [ 0. ] * len(eps_N_vals)
colors = plt.cm.rainbow( np.linspace(0, 1, len(eps_N_vals)) )
for i in range( len( eps_N_vals ) ):
    try:
        # Get the directory of the data
        directory_data = directory_results + 'eps_' + str( eps_N_vals[i][0] ) + "_rmin_" + str( eps_N_vals[i][1] ) + "_racc" + "/Analysis/"
        # Get the data
        dataMdot = np.loadtxt( directory_data + "Acc_vs_Time.dat", skiprows=1 )
        # dataMdot = np.loadtxt( directory_data + "Average_vs_Time.dat", skiprows=1 )

        # Compute the constants to normalize the data
        MdotBHL = 4 * np.pi * rho0 / (v_inf**3)
        Mdot0 = 4 * np.pi * rho0

        # Last value of time
        last_t_val[ i ] = dataMdot[ -1, 0 ]
        # Compute the average of the last 5% of values
        nr_vals_average = int( prop_average * len(dataMdot) )
        value_Mdot_avg = sum( dataMdot[ -nr_vals_average :, 1 ] ) / nr_vals_average

        values_Mdot_MdotBHL[ i ]   = value_Mdot_avg / MdotBHL
        values_log_Mdot_Mdot0[ i ] = np.log10( value_Mdot_avg / Mdot0 );

        # Plot the data
        plt.plot( dataMdot[:, 0], dataMdot[:, 1] / MdotBHL,
                  label="eps = {}, r_min/r_acc = {}".format( eps_N_vals[i][0], eps_N_vals[i][1] ),
                  c=colors[i]
                )

        # Plot also a line with the average
        plt.plot( ( dataMdot[-nr_vals_average, 0], dataMdot[-1, 0] ),
                  ( values_Mdot_MdotBHL[i], values_Mdot_MdotBHL[i] ),
                  '-.',
                  # c=colors[i]
                  c="k"
                )
    except:
        continue

# Print the values of Mdot computed
for i in range( len( eps_N_vals ) ):
    print("epsilon = {}, r_min/r_acc = {}, t_final = {}, {}% of values for average".format( eps_N_vals[i][0], eps_N_vals[i][1], last_t_val[ i ], 100*prop_average ) )
    print("\t{:40s}{}".format("Average Mdot / Mdot_BHL", values_Mdot_MdotBHL[ i ]) )
    print("\t{:40s}{}".format("Average log(Mdot / Mdot_0)", values_log_Mdot_Mdot0[ i ]) )

plt.xlabel( "time" )
plt.ylabel( "Mdot/Mdot_BHL" )
plt.legend( loc="best" )

# Save the results to a file
if save_results:
    print("Saving the average values to a file")
    with open( base_directory_results + "results_tests/results_vinf_" + v_inf_str + "_several_rmin.csv", 'w' ) as f:
        # The first line is the parameter in each column
        f.write( "epsilon,r_min/r_acc,Mdot_over_MdotBHL,log_Mdot_over_Mdot0\n" )
        # Write the data
        for i in range( len( eps_N_vals ) ):
            f.write( "{},{},{}\n".format( eps_N_vals[i][0], eps_N_vals[i][1],
                                        values_Mdot_MdotBHL[i],
                                        values_log_Mdot_Mdot0[i]
                                      ) )
    # Save also the plot
    plot_dir = base_directory_results + "results_tests/Mdot_over_time_vinf_" + v_inf_str + "_several_rmin.png"
    plt.savefig( plot_dir, dpi=250 )

plt.show()

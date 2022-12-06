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
values_log_Mdot_Mdot0 = [ 0. ] * len(eps_vals)
last_t_val            = [ 0. ] * len(eps_vals)
colors = plt.cm.rainbow( np.linspace(0, 1, len(eps_vals)) )
for i in range( len( eps_vals ) ):
    try:
        # Get the directory of the data
        directory_data = directory_results + 'eps_' + str( eps_vals[ i ] ) + "_cou_10" + "/Analysis/"
        # Get the data
        dataMdot_const_r = np.loadtxt( directory_data + "Acc_vs_Time.dat", skiprows=1 )
        dataMdot = np.loadtxt( directory_data + "Average_vs_Time.dat", skiprows=1 )

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
                  label="epsilon = {}".format( eps_vals[i] ),
                  c=colors[i]
                )

        # Plot the data measured at constant radius
        plt.plot( dataMdot_const_r[:, 0], dataMdot_const_r[:, 1] / MdotBHL,
                  '--',
                  c=colors[i], alpha=0.5
                )

        # Plot also a line with the average
        plt.plot( ( dataMdot[-nr_vals_average, 0], dataMdot[-1, 0] ),
                  ( values_Mdot_MdotBHL[i], values_Mdot_MdotBHL[i] ),
                  '-.',
                  # c=colors[i]:
                  c="k"
                )
    except:
        continue

# Print the values of Mdot computed
for i in range( len( eps_vals ) ):
    print("epsilon = {}, t_final = {}, {}% of values for average".format( eps_vals[ i ], last_t_val[ i ], 100*prop_average ) )
    print("\t{:40s}{}".format("Average Mdot / Mdot_BHL", values_Mdot_MdotBHL[ i ]) )
    print("\t{:40s}{}".format("Average log(Mdot / Mdot_0)", values_log_Mdot_Mdot0[ i ]) )

plt.xlabel( "time" )
plt.ylabel( "Mdot/Mdot_BHL" )
plt.legend( loc="best" )

# Save the results to a file
if save_results:
    print("Saving the average values to a file")
    with open( base_directory_results + "results_vinf_" + v_inf_str + ".csv", 'w' ) as f:
        # The first line is the parameter in each column
        f.write( "epsilon,Mdot_over_MdotBHL,log_Mdot_over_Mdot0\n" )
        # Write the data
        for i in range( len( eps_vals ) ):
            f.write( "{},{},{}\n".format( eps_vals[i],
                                        values_Mdot_MdotBHL[i],
                                        values_log_Mdot_Mdot0[i]
                                      ) )
    # Save also the plot
    plot_dir = base_directory_results + "Mdot_over_time_vinf_" + v_inf_str + ".png"
    plt.savefig( plot_dir, dpi=250 )

plt.show()

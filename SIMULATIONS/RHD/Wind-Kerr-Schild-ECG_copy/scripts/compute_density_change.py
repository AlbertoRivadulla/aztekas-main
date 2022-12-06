import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import re

if len(sys.argv) == 1:
    print("Run the script as:")
    print("\t python compute_density_change.py [save_results: 0 or 1] [results_directory]")

# Value of the constant rho_0
rho0 = 10**(-10)

# prop_average = 0.05
# Boolean variable that determines whether or not saving the results
save_results = False
if len(sys.argv) > 1:
    if int(sys.argv[1]) == 1:
        save_results = True

# Base directory of the results
base_directory_results = './results_ECG/K_4over3'

# Get the values of the velocities
v_inf_vals = []
for subdir in os.listdir( base_directory_results ):
    match = re.findall( r"vinf_0_([0-9]+)$", subdir )
    if len( match ) > 0:
        v_inf_vals.append( [ "0_" + match[0], float(match[0]) / 10 ] )
v_inf_vals.sort( key=lambda x: x[1] )
v_inf_vals_nr = [ el[1] for el in v_inf_vals ]

# Get the values of epsilon for the first velocity, assuming they are all the same
eps_vals = []
for subdir in os.listdir( base_directory_results + "/vinf_" + v_inf_vals[0][0] ):
    match = re.findall( r"eps_([0-9]+)$", subdir )
    if len( match ) > 0:
        eps_vals.append( int( match[0] ) )
eps_vals.sort()

# For each value of v_inf and epsilon, get the relation between the densities
#   First index: epsilon
#   Second index: v_fin
# density_relations = [ [0] * len(v_inf_vals) for _ in range(len(eps_vals)) ]
density_relations = np.zeros( ( len(eps_vals), len(v_inf_vals) ) )
for i in range(len(eps_vals)):
    for j in range(len(v_inf_vals)):
        # Base path for the files with the results
        base_files_path = base_directory_results + "/vinf_" + v_inf_vals[j][0] \
                          + "/eps_" + str( eps_vals[ i ] )
        # Find the last file in the directory
        last_file = 0
        for file in os.listdir( base_files_path ):
            match = re.findall( r"wind_([0-9]+)\.dat$", file )
            if len(match) > 0:
                last_file = max( last_file, int(match[0]) )

        # Load the data from the file
        # The data columns are:
        #   x1(r), x2(theta), U0(rho), U1(pressure), U2(v_r), U3(v_theta), sqrt( f(r) )
        file_path = base_files_path + "/wind_" + str(last_file) + ".dat"
        data = np.loadtxt( file_path, skiprows=6 )

        # Find the value of theta closest to theta = pi
        # This is the maximum value, since theta is in the interval (0, pi)
        closest_theta = max( data[:, 1] )

        # Find the maximum density at theta = pi
        max_density_in_pi = 0
        for point in data:
            if point[1] == closest_theta:
                max_density_in_pi = max( point[2], max_density_in_pi )

        # Add this to the list of densities
        # density_relations[i, j] = max_density_in_pi / rho0
        density_relations[i, j] = np.log10( max_density_in_pi / rho0 )

# Print the results to the screen
table = "\n{:9}".format( "v_inf\\eps" )
table_csv = "000"
for i in range( len( eps_vals ) ):
    table += "{:9}".format( eps_vals[i] )
    table_csv += ",{}".format( eps_vals[i] )
for j in range( len( v_inf_vals_nr ) ):
    table += "\n"
    table_csv += "\n"
    table += "{:9}".format( v_inf_vals_nr[ j ] )
    table_csv += "{}".format( v_inf_vals_nr[ j ] )
    for i in range( len( eps_vals ) ):
        table += "{:9.6}".format( density_relations[ i, j ] )
        table_csv += ",{}".format( density_relations[ i, j ] )
table += "\n"
print("\nValues of log_10( rho_max / rho_0 ):")
print(table)

# Create the figure
# fig, ax = plt.subplots( len(eps_vals), figsize=(7, 7) )
fig = plt.figure( figsize=(6, 4) )

# Plot the data
colors = plt.cm.rainbow( np.linspace(0, 1, len(eps_vals)) )
for i in range( len( eps_vals ) ):
    plt.plot( v_inf_vals_nr, density_relations[ i ],
              '-o',
              label="epsilon = {}".format( eps_vals[ i ] ),
              c=colors[i]
            )

plt.xlabel( "v_inf" )
plt.ylabel( "log10(rho_max/rho_0)" )
plt.legend( loc="best" )

# Save the results to a file
if save_results:
    print("Saving the plot to a file")
    # Save also the plot
    plot_dir = base_directory_results + "/density_change.png"
    plt.savefig( plot_dir, dpi=250 )

    # Write the data to a csv file
    with open( base_directory_results + "/density_change.csv", "w" ) as f:
        f.write( table_csv )

plt.show()

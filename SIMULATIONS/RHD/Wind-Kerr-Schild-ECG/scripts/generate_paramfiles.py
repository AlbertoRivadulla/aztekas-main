import sys
import os

paramfile_string = '''
% OUTPUT FILE AND DIRECTORY
outputdirectory = ./results_ECG/K_{K_str}/vinf_{vinf_str}/eps_{eps_val}/
outputfile      = wind_

% RESTART SIMULATION (1 -> true, 0 -> false)
restart_simulation = 0
restartfile = ./results_ECG/K_{K_str}/vinf_{vinf_str}/eps_{eps_val}/wind_30.dat
restart_filecount = 30

% Define mesh
Nx1 = {Nx1}
Nx2 = {Nx2}
Nx3 = 0

% Define domain limits
x1min = {r_min}
x1max = {r_max}
x2min = 0.0
x2max = 1.0
x3min = 0.0
x3max = 1.0

% Time parameters
tmax = {tmax}
timefile = {timefile}
cou = {cou}

% Polytorpic index
K = {K_val}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Initial conditions %%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

density_0  =  1.0e-10
pressure_0 =  1.0

%%%%%%%%%%%%%%%%%%%%%%%
% MASS ACCRETION RATE %
%%%%%%%%%%%%%%%%%%%%%%%

% Error threshold
MDOT_ERR = 1.0e-04
% Interval of time for computing the accretion rate
Mdot_tprint = 0.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Parameters for the ECG BH %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

epsilon_cubic = {eps_val}.
polynomial_f_file = Polynomials_f_ECG/f(u)_epsilon_{eps_val}..csv
'''

################################################################################
################################################################################

# Values of the ECG coupling epsilon
epsilon_vals = ( 0, 5, 20, 500 )
# Values of the horizon radius in the variable Z
Zh_vals = ( 1., 0.6330307056015014, 0.5209896379277313, 0.3175934799904413 )
# Values of v_inf
v_inf_vals = ( 0.2, 0.5, 0.6, 0.7, 0.8, 0.9 )
# Mach number
Mach = 5
# Polytropic index
K_str = '4/3'
# Number of points in each direction
N_r     = 400
N_theta = 400

# Values of tmax and cou
# One per value of v_inf
tmax_vals = [ 2000, 1500, 500, 400, 150, 100 ]
cou_vals  = [ 0.40, 0.25, 0.25, 0.2, 0.2, 0.1 ]

# Directory for the files
paramfiles_directory = './paramfiles_ECG_gen'
if len(sys.argv) > 1:
    paramfiles_directory = str( sys.argv[1] )
if paramfiles_directory[-1] != '/':
    paramfiles_directory += '/'
# Create the directory if it does not exist
if not os.path.exists( paramfiles_directory ):
    os.makedirs( paramfiles_directory )

# Base name for the files
paramfiles_base_name = paramfiles_directory + 'wind_ECG_{epsilon}_vinf_{vinf}.param'

epsilon_vals = [0]
v_inf_vals = [0.2]

# Generate all the parameter files
for i in range( len( epsilon_vals ) ):
    for j in range( len( v_inf_vals ) ):
        v_inf = v_inf_vals[ j ]
        # Compute the accretion radius
        r_acc = 1. / ( Zh_vals[ i ] * v_inf**2 * ( 1 + 1 / Mach**2 ) )
        # Compute the minimun and maximum radii of the domain
        r_min = r_acc / 2
        r_max = 10 * r_acc
        if v_inf == 0.2:
            r_min = r_acc / 4

        # Create a dictionary with the values
        dic_vals = {
                    'K_str' : K_str.replace('/', 'over'),
                    'K_val' : eval( K_str ),
                    'vinf_str' : str( v_inf ).replace( '.', '_' ),
                    'eps_val' : str( epsilon_vals[ i ] ),
                    'Nx1' : N_r,
                    'Nx2' : N_theta,
                    'r_min' : r_min,
                    'r_max' : r_max,
                    'tmax' : tmax_vals[ j ],
                    'timefile' : 20 / tmax_vals[ j ],
                    'cou' : cou_vals[ j ]
                    }

        # Write the string to a file
        file_name = paramfiles_base_name.format( epsilon=dic_vals['eps_val'],
                                                 vinf=dic_vals['vinf_str'] )
        with open( file_name, 'w' ) as f:
            f.write( paramfile_string.format( **dic_vals ) )

        #
        # # Print the results
        # print( "\n---------" )
        # print( "v_inf = {}\n".format( v_inf ) )
        # print( "x1min = {}".format( r_min ) )
        # print( "x1max = {}".format( r_max ) )

# K_str
# K_val
# vinf_str
# eps_val
# Nx1
# Nx2
# r_min
# r_max
# tmax
# timefile
# cou


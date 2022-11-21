
# Values of the ECG coupling epsilon
epsilon_vals = ( 0., 5., 20., 500. )
# Values of the horizon radius in the variable Z
Zh_vals = ( 1., 0.6330307056015014, 0.5209896379277313, 0.3175934799904413 )

# Values of v_inf
v_inf_vals = ( 0.2, 0.5, 0.6, 0.7, 0.8, 0.9 )

# Mach number
Mach = 5

for i in range(len(epsilon_vals)):
    print( "\n\n=====================================\n\n" )
    print( "epsilon = {}\n".format( epsilon_vals[ i ] ) )
    for v_inf in v_inf_vals:
        # Compute the accretion radius
        r_acc = 1. / ( Zh_vals[ i ] * v_inf**2 * ( 1 + 1 / Mach**2 ) )

        # Compute the minimun and maximum radii of the domain
        r_min = r_acc / 2
        r_max = 10 * r_acc

        if v_inf == 0.2:
            r_min = r_acc / 4
            # r_min = 1 / Zh_vals[ i ]

        # Print the results
        print( "\n---------" )
        print( "v_inf = {}\n".format( v_inf ) )
        print( "x1min = {}".format( r_min ) )
        print( "x1max = {}".format( r_max ) )


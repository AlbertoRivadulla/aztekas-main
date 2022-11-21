#include "main.h"

// Function to read the data for the interpolation of f(r) in the metric
void Read_Interpolation_f_Metric()
{
    int   BUFFER_SIZE = 612;
    char t_key[BUFFER_SIZE], t_value[BUFFER_SIZE], t_firstChar;
 
    // Read the polynomial file from the parameter file
    char polynomial_file_path[40];
    FILE* paramfile = fopen (paramfile_name, "r");
    if (paramfile == NULL)
    {
        fprintf(stderr, "Error opening parameter file: %s\n", paramfile_name);
        exit(EXIT_FAILURE);
    }
    // Look only for the line with the location of the polynomial data and the 
    // coupling of the cubic terms
    while(fscanf(paramfile, " %c", &t_firstChar) == 1 )
    {
        if(t_firstChar != '/' && t_firstChar != '%')
        {
            // Not a comment so read the key value pair
            // Move back one space in the input stream with seek
            fseek(paramfile, -1, SEEK_CUR);
 
            if(fscanf (paramfile, "%s = %s", t_key, t_value) == 2)
            {
                if(strcmp(t_key, "epsilon_cubic")==0)
                   epsilon_cubic = atof(t_value);
                if(strcmp(t_key, "polynomial_f_file")==0)
                   strcpy( polynomial_file_path, t_value );
            }
        }
    }
    fclose(paramfile);         
    
    // Open the file to read the polynomial
    FILE* polynomial_file = fopen(polynomial_file_path, "r");
    if (polynomial_file == NULL)
    {
        fprintf(stderr, "Error opening the file with the interpolation: %s\n", 
                polynomial_file_path);
        exit(EXIT_FAILURE);
    }
    else
        printf("Reading the metric coefficients from %s\n", polynomial_file_path);
 
    // Count the lines in the file
    // This is equal to the number of intervals in the interpolation
    //
    //
    // Here I should also count the number of elements in each line, to know the 
    // order of the polynomial.
    //
    //
    int ch;
    interp_f_ECG.n_points = 0;
    while ( ( ch = fgetc( polynomial_file ) ) != EOF )
    {
        if ( ch == '\n' )
            interp_f_ECG.n_points++;
    }
    // The first two lines correspond to the horizon position Z_h and f'(Z_h)
    interp_f_ECG.n_points -= 2;
    
    // Move the pointer back to the beginning of the file
    fseek( polynomial_file, 0, SEEK_SET );

    // Reserve memory for the limits of the intervals and the coefficients
    interp_f_ECG.limits = (double*)malloc( interp_f_ECG.n_points * sizeof( double ) );
    /* interp_f_ECG.coeffs = (double*)malloc( 4 * interp_f_ECG.n_points * sizeof( double ) ); */
    interp_f_ECG.coeffs = (double*)malloc( 6 * interp_f_ECG.n_points * sizeof( double ) );

    // The following are needed to translate the strings to numbers
    char* ptr;
    char line_buffer[ 1024 ];

    // Read the horizon position and the derivative f'(Z_h) from the first two lines
    double Z_hor;
    double f_prime_Z_hor;
    fgets( line_buffer, 1024, polynomial_file );
    Z_hor = strtod( line_buffer, &ptr );
    fgets( line_buffer, 1024, polynomial_file );
    f_prime_Z_hor = strtod( line_buffer, &ptr );

    // Convert these to the coordinates r = 2M/Z
    double M = Black_Hole_Mass;
    interp_f_ECG.r_hor = 2. * M / Z_hor;
    interp_f_ECG.f_prime_r_hor = - Z_hor*Z_hor / (2. * M) * f_prime_Z_hor;

    // Read the following lines of the file
    int row = 0;
    int column = 0;
    // fgets reads a line from the file stream, and stores it in the string pointer
    // line_buffer. It stops when it reaches a \n or has read 1024 characters, in this case.
    while( fgets( line_buffer, 1024, polynomial_file ) )
    {
        column = 0;

        // Split the data
        char* value = strtok( line_buffer, "," );
        
        while ( value )
        {
            // The three first columns are the coefficients of the polynomial
            /* if ( column <= 3 ) */
            if ( column <= 5 )
                /* interp_f_ECG.coeffs[ row*5 + column ] = strtod( value, &ptr ); */
                interp_f_ECG.coeffs[ row*7 + column ] = strtod( value, &ptr );
            else
                interp_f_ECG.limits[ row ] = strtod( value, &ptr );

            // Continue tokenizing the string that was passed first
            value = strtok( NULL, "," );
            column++;
        }
        row++;
    }
 
    // Store the minimum and maximum values of x in the interpolation struct
    interp_f_ECG.xmin = 0.;
    interp_f_ECG.xmax = interp_f_ECG.limits[ interp_f_ECG.n_points - 1 ];
 
    // Close the file
    fclose( polynomial_file );
}

// Function f(r) in the metric
double f_Metric_From_Interp( const double M, const double r )
{
    /* if ( epsilon_cubic > 0. ) */
    /* { */
    // Compute the variable Z
    double Z = 2. * M / r;

    // Find which interval Z belongs to
    int i_interval = 0;
    while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 )
        i_interval++;

    // Compute the value with the cubic polynomial in that interval
    return   interp_f_ECG.coeffs[i_interval*7 + 0] 
           + interp_f_ECG.coeffs[i_interval*7 + 1]*Z 
           + interp_f_ECG.coeffs[i_interval*7 + 2]*Z*Z
           + interp_f_ECG.coeffs[i_interval*7 + 3]*Z*Z*Z
           + interp_f_ECG.coeffs[i_interval*7 + 4]*Z*Z*Z*Z
           + interp_f_ECG.coeffs[i_interval*7 + 5]*Z*Z*Z*Z*Z;
    /* return   interp_f_ECG.coeffs[i_interval*5 + 0]  */
    /*        + interp_f_ECG.coeffs[i_interval*5 + 1]*Z  */
    /*        + interp_f_ECG.coeffs[i_interval*5 + 2]*Z*Z */
    /*        + interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z*Z; */
    /* } */
    /*  */
    /* // Solution in GR */
    /* return 1. - 2. * M / r; */
}

// Derivative of f(r) in the metric
double f_Prime_Metric_From_Interp( const double M, const double r )
{
    /* if ( epsilon_cubic > 0. ) */
    /* { */
    // Compute the variable Z
    double Z = 2. * M / r;

    // Find which interval u belongs to
    int i_interval = 0;
    while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 )
        i_interval++;

    // Compute the value with the cubic polynomial in that interval
    /* return - Z*Z * 0.5 / M * ( interp_f_ECG.coeffs[i_interval*5 + 1] */
    /*                            + 2. * interp_f_ECG.coeffs[i_interval*5 + 2]*Z */
    /*                            + 3. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z ); */
    return - Z*Z * 0.5 / M * ( interp_f_ECG.coeffs[i_interval*7 + 1]
                               + 2. * interp_f_ECG.coeffs[i_interval*7 + 2]*Z
                               + 3. * interp_f_ECG.coeffs[i_interval*7 + 3]*Z*Z
                               + 4. * interp_f_ECG.coeffs[i_interval*7 + 4]*Z*Z*Z
                               + 5. * interp_f_ECG.coeffs[i_interval*7 + 5]*Z*Z*Z*Z );
    /* } */
    /*  */
    /* // Solution in GR */
    /* return 2. * M / (r * r); */
}

/* // Second derivative of f(r) in the metric */
/* double f_Prime_Prime_Metric_From_Interp( const double M, const double r ) */
/* { */
/*     // Compute the variable Z */
/*     double Z = 2. * M / r; */
/*  */
/*     // Find which interval u belongs to */
/*     int i_interval = 0; */
/*     while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 ) */
/*         i_interval++; */
/*  */
/*     // Compute the value with the cubic polynomial in that interval */
/*     #<{(| double f_p_Z  = interp_f_ECG.coeffs[i_interval*5 + 1] |)}># */
/*     #<{(|                 + 2. * interp_f_ECG.coeffs[i_interval*5 + 2]*Z |)}># */
/*     #<{(|                 + 3. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z; |)}># */
/*     #<{(| double f_pp_Z = 2. * interp_f_ECG.coeffs[i_interval*5 + 2]  |)}># */
/*     #<{(|                 + 6. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z; |)}># */
/*     double f_p_Z  = interp_f_ECG.coeffs[i_interval*7 + 1] */
/*                     + 2. * interp_f_ECG.coeffs[i_interval*7 + 2]*Z */
/*                     + 3. * interp_f_ECG.coeffs[i_interval*7 + 3]*Z*Z */
/*                     + 4. * interp_f_ECG.coeffs[i_interval*7 + 4]*Z*Z*Z */
/*                     + 5. * interp_f_ECG.coeffs[i_interval*7 + 5]*Z*Z*Z*Z; */
/*     double f_pp_Z =   2. * interp_f_ECG.coeffs[i_interval*7 + 2]  */
/*                     + 6. * interp_f_ECG.coeffs[i_interval*7 + 3]*Z */
/*                     + 12. * interp_f_ECG.coeffs[i_interval*7 + 4]*Z*Z */
/*                     + 20. * interp_f_ECG.coeffs[i_interval*7 + 5]*Z*Z*Z; */
/*  */
/*     return Z*Z*Z / (2. * M) * f_p_Z + Z*Z*Z*Z / (4. * M) * f_pp_Z; */
/* } */

// Compute the values of the functions in the metric at a point in the grid
void Compute_Functions_Metric_In_Point( double r , int index )
{
    // Get f(r), f'(r) and f''(r) at this point
    double f   = f_Metric_From_Interp( Black_Hole_Mass, r );
    double fp  = f_Prime_Metric_From_Interp( Black_Hole_Mass, r );
    /* double fpp = f_Prime_Prime_Metric_From_Interp( Black_Hole_Mass, r ); */
    // Compute the factor r_h*f'(r_h)
    /* double rh_fprh = interp_f_ECG.r_hor * interp_f_ECG.f_prime_r_hor; */
    double rh  = interp_f_ECG.r_hor;

    // If f(r) is equal to zero, I am at the horizon, and some of the expressions
    // blow up if the limit is not taken with caution. I do this explicitly
    if ( f == 0 )
    {
        printf("Trying to evaluate at the horizon. IMPLEMENT THIS!\n");
        exit(EXIT_FAILURE);
    }

    // Compute the functions in the metric
    
    /* alpha_vals[ index ]        = sqrt( f / ( 1. - r*r * fp*fp / ( rh_fprh*rh_fprh ) ) ); */
    /* beta_r_con_vals[ index ]   = r * f * fp / ( rh_fprh * ( 1.  */
    /*                              - r*r * fp*fp / ( rh_fprh*rh_fprh ) ) ); */
    /* gamma_rr_cov_vals[ index ] = ( 1. - r*r * fp*fp / ( rh_fprh*rh_fprh ) ) / f; */
    /* beta_r_cov_vals[ index ]   = beta_r_con_vals[ index ] * gamma_rr_cov_vals[ index ]; */
    /* gamma_rr_con_vals[ index ] = 1. / gamma_rr_cov_vals[ index ]; */

    /* alpha_vals[ index ]        = sqrt( r*r * f / ( r*r - rh*rh ) ); */
    /* beta_r_con_vals[ index ]   = r * rh * f / ( r*r - rh*rh ); */
    /* gamma_rr_cov_vals[ index ] = ( r*r - rh*rh ) / ( r*r * f ); */
    /* beta_r_cov_vals[ index ]   = beta_r_con_vals[ index ] * gamma_rr_cov_vals[ index ]; */
    /* gamma_rr_con_vals[ index ] = 1. / gamma_rr_cov_vals[ index ]; */

    alpha_vals[ index ]        = sqrt( 1. / ( 2. - f ) );
    beta_r_con_vals[ index ]   = ( -1. + f ) / ( -2. + f );
    gamma_rr_cov_vals[ index ] = 2. - f;
    beta_r_cov_vals[ index ]   = beta_r_con_vals[ index ] * gamma_rr_cov_vals[ index ];
    gamma_rr_con_vals[ index ] = 1. / gamma_rr_cov_vals[ index ];
    
    // Compute the derivatives of the functions in the metric

    /* der_r_alpha_vals[ index ]        = pow( f / ( 1. - r*r*fp*fp / ( rh_fprh*rh_fprh ) ) , 3./2. )  */
    /*                                    * fp / ( 2. * rh_fprh*rh_fprh * f*f ) */
    /*                                    * ( rh_fprh*rh_fprh - r*r*fp*fp +  */
    /*                                        2. * r * f * ( fp + r * fpp )  */
    /*                                      ); */
    /* der_r_beta_con_vals[ index ]     = rh_fprh / ( pow( rh_fprh*rh_fprh - r*r * fp*fp , 2. ) ) */
    /*                                    * ( rh_fprh*rh_fprh * r * fp*fp  */
    /*                                        - r*r*r * fp*fp*fp*fp */
    /*                                        + f * ( rh_fprh*rh_fprh + r*r * fp*fp )  */
    /*                                            * ( fp + r * fpp )  */
    /*                                      ); */
    /* der_r_gamma_rr_cov_vals[ index ] = - fp / ( rh_fprh*rh_fprh * f*f ) */
    /*                                    * ( rh_fprh*rh_fprh - r*r*fp*fp +  */
    /*                                        2. * r * f * ( fp + r * fpp )  */
    /*                                      ); */

    /* der_r_alpha_vals[ index ]        = r * ( -2. * rh*rh * f + r * (r*r - rh*rh) * fp ) */
    /*                                      / ( ( r*r - rh*rh ) * ( r*r - rh*rh ) ) */
    /*                                      / ( 2. * alpha_vals[ index ] ); */
    /* der_r_beta_con_vals[ index ]     = rh * ( -f * (r*r + rh*rh) + r * (r*r - rh*rh) * fp ) */
    /*                                       / ( ( r*r - rh*rh ) * ( r*r - rh*rh ) ); */
    /* der_r_gamma_rr_cov_vals[ index ] = ( 2 * rh*rh * f - r * (r*r - rh*rh) * fp ) */
    /*                                    / ( r*r*r * f*f );  */

    der_r_alpha_vals[ index ]        = fp / ( ( -2. + f ) * ( -2. + f ) )
                                          / ( 2. * alpha_vals[ index ] );
    der_r_beta_con_vals[ index ]     = -fp / ( ( -2. + f ) * ( -2. + f ) );
    der_r_gamma_rr_cov_vals[ index ] = -fp;
                                       
    /* printf("%f (%f) %f %f %f %f %f --- %f %f %f\n", r, f, */
    /*                                     alpha_vals[ index ], */
    /*                                     beta_r_con_vals[ index ], */
    /*                                     gamma_rr_cov_vals[ index ], */
    /*                                     beta_r_cov_vals[ index ], */
    /*                                     gamma_rr_con_vals[ index ], */
    /*                                     der_r_alpha_vals[ index ], */
    /*                                     der_r_beta_con_vals[ index ], */
    /*                                     der_r_gamma_rr_cov_vals[ index ] */
    /*                                     ); */
}

// Function to compute the values of f(r) and f'(r) in the grid
void Compute_f_In_Grid()
{
    printf("Computing the metric functions in the grid\n\n");

    // Allocate the arrays
    alpha_vals              = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    beta_r_con_vals         = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    beta_r_cov_vals         = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    gamma_rr_con_vals       = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    gamma_rr_cov_vals       = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    der_r_alpha_vals        = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    der_r_beta_con_vals     = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    der_r_gamma_rr_cov_vals = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );

    // Compute the values in the grid
    for ( int i = 0; i <= Nx1; ++i )
    {
        // Compute the values of the functions of the metric at this point
        Compute_Functions_Metric_In_Point( grid.X1[ i ], i );

        // Compute the values of the functions of the metric at the point with positive displacement
        Compute_Functions_Metric_In_Point( grid.X1p[ i ], i + Nx1 + 1 );

        // Compute the values of the functions of the metric at the point with negative displacement
        Compute_Functions_Metric_In_Point( grid.X1m[ i ], i + 2*Nx1 + 2 );
    }
}

// Function to clear the resources of the interpolated metric
void Free_Interpolated_Metric()
{
    free( interp_f_ECG.limits );
    free( interp_f_ECG.coeffs );

    free( alpha_vals              );
    free( beta_r_con_vals         );
    free( beta_r_cov_vals         );
    free( gamma_rr_con_vals       );
    free( gamma_rr_cov_vals       );
    free( der_r_alpha_vals        );
    free( der_r_beta_con_vals     );
    free( der_r_gamma_rr_cov_vals );
}

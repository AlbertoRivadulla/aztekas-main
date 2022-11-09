#include"main.h"

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
    
    // Move the pointer back to the beginning of the file
    fseek( polynomial_file, 0, SEEK_SET );

    // Reserve memory for the limits of the intervals and the coefficients
    interp_f_ECG.limits = (double*)malloc( interp_f_ECG.n_points * sizeof( double ) );
    interp_f_ECG.coeffs = (double*)malloc( 4 * interp_f_ECG.n_points * sizeof( double ) );

    // Read the lines of the csv file
    char line_buffer[ 1024 ];
    int row = 0;
    int column = 0;
    // fgets reads a line from the file stream, and stores it in the string pointer
    // line_buffer. It stops when it reaches a \n or has read 1024 characters, in this case.
    while( fgets( line_buffer, 1024, polynomial_file ) )
    {
        column = 0;

        // Split the data
        char* value = strtok( line_buffer, "," );
        // Pointer needed to translate the strings to double numbers
        char* ptr;
        
        while ( value )
        {
            // The three first columns are the coefficients of the polynomial
            if ( column <= 3 )
                interp_f_ECG.coeffs[ row*5 + column ] = strtod( value, &ptr );
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
double f_Metric( const double M, const double r )
{
    if ( epsilon_cubic > 0. )
    {
        // Compute the variable Z
        double Z = 2. * M / r;
 
        /* printf("%f -- ", Z); */

        // Find which interval Z belongs to
        int i_interval = 0;
        while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 )
            i_interval++;

        /* printf("%d -- ", i_interval); */
        /* for ( int i = 0; i < 4; ++i ) */
        /*    printf("%f # ", interp_f_ECG.coeffs[i_interval*5 + i]); */

        /* printf("%f -- ", Z); */
 
        // Compute the value with the cubic polynomial in that interval
        return   interp_f_ECG.coeffs[i_interval*5 + 0] 
               + interp_f_ECG.coeffs[i_interval*5 + 1]*Z 
               + interp_f_ECG.coeffs[i_interval*5 + 2]*Z*Z
               + interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z*Z;
    }

    // Solution in GR
    return 1. - 2. * M / r;
}

// Derivative of f(r) in the metric
double f_Prime_Metric( const double M, const double r )
{
    if ( epsilon_cubic > 0. )
    {
        // Compute the variable Z
        double Z = 2. * M / r;

        // Find which interval u belongs to
        int i_interval = 0;
        while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 )
            i_interval++;

        // Compute the value with the cubic polynomial in that interval
        return - Z*Z * 0.5 / M * ( interp_f_ECG.coeffs[i_interval*5 + 1]
                                   + 2. * interp_f_ECG.coeffs[i_interval*5 + 2]*Z
                                   + 3. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z );
    }

    // Solution in GR
    return 2. * M / (r * r);
}

void Get_Metric_Components(gauge_ *local_grid)
{
#if COORDINATES == CARTESIAN

   printf("The metric is not defined in cartesian coordinates.\n");

#elif COORDINATES == CYLINDRICAL

   printf("The metric is not defined in cylindrical coordinates.\n");

#elif COORDINATES == SPHERICAL

   double r     = local_grid->x[1];
   double theta = local_grid->x[2]; 
   double M     = Black_Hole_Mass;

   // Compute the value of f(r) at this point
   double f_Metric_val = f_Metric( M, r );

   local_grid->lapse = sqrt( f_Metric_val );

   local_grid->beta_con[0] = 0.0;
   local_grid->beta_con[1] = 0.0;
   local_grid->beta_con[2] = 0.0;

   /* printf( "\n%f %f\n", r, f_Metric_val ); */

   local_grid->gamma_con[0][0] = f_Metric_val;
   local_grid->gamma_con[0][1] = 0.0;
   local_grid->gamma_con[0][2] = 0.0;
   local_grid->gamma_con[1][0] = 0.0;
   local_grid->gamma_con[1][1] = 1.0/(r*r);
   local_grid->gamma_con[1][2] = 0.0;
   local_grid->gamma_con[2][0] = 0.0;
   local_grid->gamma_con[2][1] = 0.0;
   local_grid->gamma_con[2][2] = 1.0/(r*r*sin(theta)*sin(theta));

   local_grid->dety = r*r * fabs(sin(theta)) / sqrt( f_Metric_val );

#endif
}

void Gauge_Derivatives(der_gauge_ *der, gauge_ *local_grid)
{
#if COORDINATES == CARTESIAN

   printf("The metric is not defined in cartesian coordinates.\n");

#elif COORDINATES == CYLINDRICAL

   printf("The metric is not defined in cylindrical coordinates.\n");

#elif COORDINATES == SPHERICAL

   double r     = local_grid->x[1];
   double theta = local_grid->x[2]; 
   double M     = Black_Hole_Mass;

   // Compute f(r) and its derivative f'(r) at this point
   double f_Metric_val       = f_Metric( M, r );
   double f_Prime_Metric_val = f_Prime_Metric( M, r );

   der->dlapse[0] = 0.5 * f_Prime_Metric_val / sqrt( f_Metric_val );
   der->dlapse[1] = 0.0;
   der->dlapse[2] = 0.0;

   der->dbeta[0][0] = 0.0;
   der->dbeta[0][1] = 0.0;
   der->dbeta[0][2] = 0.0;
   der->dbeta[1][0] = 0.0;
   der->dbeta[1][1] = 0.0;
   der->dbeta[1][2] = 0.0;
   der->dbeta[2][0] = 0.0;
   der->dbeta[2][1] = 0.0;
   der->dbeta[2][2] = 0.0;

   der->dgam[0][0][0] = - f_Prime_Metric_val / ( f_Metric_val*f_Metric_val );
   der->dgam[0][0][1] = 0.0;
   der->dgam[0][0][2] = 0.0;
   der->dgam[0][1][0] = 0.0;
   der->dgam[0][1][1] = 2.0*r;
   der->dgam[0][1][2] = 0.0;
   der->dgam[0][2][0] = 0.0;
   der->dgam[0][2][1] = 0.0;
   der->dgam[0][2][2] = 2.0*r*sin(theta)*sin(theta);

   der->dgam[1][0][0] = 0.0;
   der->dgam[1][0][1] = 0.0;
   der->dgam[1][0][2] = 0.0;
   der->dgam[1][1][0] = 0.0;
   der->dgam[1][1][1] = 0.0;
   der->dgam[1][1][2] = 0.0;
   der->dgam[1][2][0] = 0.0;
   der->dgam[1][2][1] = 0.0;
   der->dgam[1][2][2] = 2.0*r*r*cos(theta)*sin(theta);

   der->dgam[2][0][0] = 0.0;
   der->dgam[2][0][1] = 0.0;
   der->dgam[2][0][2] = 0.0;
   der->dgam[2][1][0] = 0.0;
   der->dgam[2][1][1] = 0.0;
   der->dgam[2][1][2] = 0.0;
   der->dgam[2][2][0] = 0.0;
   der->dgam[2][2][1] = 0.0;
   der->dgam[2][2][2] = 0.0;

#endif
}

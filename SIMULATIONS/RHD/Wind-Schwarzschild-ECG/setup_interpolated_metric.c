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
double f_Metric_From_Interp( const double M, const double r )
{
    if ( epsilon_cubic > 0. )
    {
        // Compute the variable Z
        double Z = 2. * M / r;

        // Find which interval Z belongs to
        int i_interval = 0;
        while ( Z > interp_f_ECG.limits[ i_interval + 1 ] && i_interval < interp_f_ECG.n_points - 1 )
            i_interval++;
 
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
double f_Prime_Metric_From_Interp( const double M, const double r )
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
        double aux = interp_f_ECG.coeffs[i_interval*5 + 1]
                                   + 2. * interp_f_ECG.coeffs[i_interval*5 + 2]*Z
                                   + 3. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z;
        return - Z*Z * 0.5 / M * ( interp_f_ECG.coeffs[i_interval*5 + 1]
                                   + 2. * interp_f_ECG.coeffs[i_interval*5 + 2]*Z
                                   + 3. * interp_f_ECG.coeffs[i_interval*5 + 3]*Z*Z );
    }

    // Solution in GR
    return 2. * M / (r * r);
}

// Function to compute the values of f(r) and f'(r) in the grid
void Compute_f_In_Grid()
{
    printf("Computing f(r) and f'(r) in the grid\n\n");
    // Allocate the arrays
    f_vals       = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );
    f_prime_vals = (double*)malloc( (Nx1 + 1) * 3 * sizeof( double ) );

    // Iterate over the grid
    for ( int i = 0; i <= Nx1; ++i )
    {
        // Compute the values in the grid
        f_vals[ i + 0 ]       = f_Metric_From_Interp( Black_Hole_Mass, grid.X1[ i ] );
        f_prime_vals[ i + 0 ] = f_Prime_Metric_From_Interp( Black_Hole_Mass, grid.X1[ i ] );

        // Compute the values in the grid with positive displacement
        f_vals[ i + Nx1 + 1 ]       = f_Metric_From_Interp( Black_Hole_Mass, grid.X1p[ i ] );
        f_prime_vals[ i + Nx1 + 1 ] = f_Prime_Metric_From_Interp( Black_Hole_Mass, grid.X1p[ i ] );

        // Compute the values in the grid with negative displacement
        f_vals[ i + 2*Nx1 + 2 ]       = f_Metric_From_Interp( Black_Hole_Mass, grid.X1m[ i ] );
        f_prime_vals[ i + 2*Nx1 + 2 ] = f_Prime_Metric_From_Interp( Black_Hole_Mass, grid.X1m[ i ] );

    }
}

// Function to clear the resources of the interpolated metric
void Free_Interpolated_Metric()
{
    free( interp_f_ECG.limits );
    free( interp_f_ECG.coeffs );

    free( f_vals );
    free( f_prime_vals );
}

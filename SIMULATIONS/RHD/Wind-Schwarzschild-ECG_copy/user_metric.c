#include"main.h"

// Function f(r) in the metric
double f_Metric( const double M, const double r )
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

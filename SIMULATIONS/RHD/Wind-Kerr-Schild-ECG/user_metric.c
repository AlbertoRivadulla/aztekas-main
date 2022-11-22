#include"main.h"

void Get_Metric_Components(gauge_ *local_grid)
{
#if COORDINATES == CARTESIAN

   printf("The metric is not defined in cartesian coordinates.\n");
   exit(EXIT_FAILURE);

#elif COORDINATES == CYLINDRICAL

   printf("The metric is not defined in cylindrical coordinates.\n");
   exit(EXIT_FAILURE);

#elif COORDINATES == SPHERICAL

   double r     = local_grid->x[1];
   double theta = local_grid->x[2]; 
   int index    = local_grid->I[0];
   /* double phi   = local_grid->x[3]; */

   /* double Sigma, Delta, rho2, a, M; */
   /* double cos2, sin2; */
   /* double r2; */
   /* double r2, a2; */

   double sin_theta = sin(theta);
   double cos_theta = cos(theta);
   if(theta == M_PI)
   {
      sin_theta =  0.0;
      cos_theta = -1.0;
   }
   double cos2  = cos_theta * cos_theta;
   double sin2  = sin_theta * sin_theta;
   double r2    = r * r;

   /* a     = Black_Hole_Spin; */
   /* M     = Black_Hole_Mass; */

   /* a2    = pow(a,2.0); */

   /* Delta = r2 - 2.0*M*r + a2; */
   /* rho2  = r2 + a2*cos2; */
   /* Sigma = pow(r2 + a2,2.0) - Delta*a2*sin2; */

   local_grid->lapse = alpha_vals[ index ];

   local_grid->beta_con[0] = beta_r_con_vals[ index ];
   local_grid->beta_con[1] = 0.0;
   local_grid->beta_con[2] = 0.0;

   local_grid->beta_cov[0] = beta_r_cov_vals[ index ];
   local_grid->beta_cov[1] = 0.0;
   local_grid->beta_cov[2] = 0.0;

   #if POLAR == FALSE

   local_grid->gamma_con[0][0] = gamma_rr_con_vals[ index ];
   local_grid->gamma_con[0][1] = 0.0;
   local_grid->gamma_con[0][2] = 0.0;
   local_grid->gamma_con[1][0] = 0.0;
   local_grid->gamma_con[1][1] = 1.0 / r2;
   local_grid->gamma_con[1][2] = 0.0;
   local_grid->gamma_con[2][0] = 0.0;
   local_grid->gamma_con[2][1] = 0.0;
   local_grid->gamma_con[2][2] = 1.0 / ( r2 * sin2 );

   local_grid->gamma_cov[0][0] = gamma_rr_cov_vals[ index ];
   local_grid->gamma_cov[0][1] = 0.0;
   local_grid->gamma_cov[0][2] = 0.0;
   local_grid->gamma_cov[1][0] = 0.0;
   local_grid->gamma_cov[1][1] = r2;
   local_grid->gamma_cov[1][2] = 0.0;
   local_grid->gamma_cov[2][0] = 0.0;
   local_grid->gamma_cov[2][1] = 0.0;
   local_grid->gamma_cov[2][2] = r2 * sin2;

   #elif POLAR == TRUE

   /* printf("The metric is not defined in polar coordinates\n"); */
   /* exit(EXIT_FAILURE); */

   local_grid->gamma_con[0][0] = gamma_rr_con_vals[ index ];
   local_grid->gamma_con[0][1] = 0.0;
   local_grid->gamma_con[0][2] = 0.0;
   local_grid->gamma_con[1][0] = 0.0;
   local_grid->gamma_con[1][1] = 1.0 / (r2 * sin2);
   local_grid->gamma_con[1][2] = 0.0;
   local_grid->gamma_con[2][0] = 0.0;
   local_grid->gamma_con[2][1] = 0.0;
   local_grid->gamma_con[2][2] = 1.0 / r2;

   local_grid->gamma_cov[0][0] = gamma_rr_cov_vals[ index ];
   local_grid->gamma_cov[0][1] = 0.0;
   local_grid->gamma_cov[0][2] = 0.0;
   local_grid->gamma_cov[1][0] = 0.0;
   local_grid->gamma_cov[1][1] = r2 * sin2;
   local_grid->gamma_cov[1][2] = 0.0;
   local_grid->gamma_cov[2][0] = 0.0;
   local_grid->gamma_cov[2][1] = 0.0;
   local_grid->gamma_cov[2][2] = r2;

   #endif

   local_grid->dety = sqrt( r2*r2 * sin2 * gamma_rr_cov_vals[ index ] );

#endif
}

void Gauge_Derivatives(der_gauge_ *der, gauge_ *local_grid)
{
#if COORDINATES == CARTESIAN

   printf("The metric is not defined in cartesian coordinates.\n");
   exit(EXIT_FAILURE);

#elif COORDINATES == CYLINDRICAL

   printf("The metric is not defined in cylindrical coordinates.\n");
   exit(EXIT_FAILURE);

#elif COORDINATES == SPHERICAL

   double r     = local_grid->x[1];
   double theta = local_grid->x[2]; 
   int index    = local_grid->I[0];
   /* double phi   = local_grid->x[3]; */

   /* double Sigma, Delta, rho2, a, M; */
   /* double cos2, sin2; */
   /* double r2, a2; */
   /*  */
   double sin_theta = sin(theta);
   double cos_theta = cos(theta);
   if(theta == M_PI)
   {
      sin_theta =  0.0;
      cos_theta = -1.0;
   }
   /*  */
   /* a     = Black_Hole_Spin; */
   /* M     = Black_Hole_Mass; */
   /*  */
   /* cos2  = pow(cos_theta,2.0); */
   double sin2  = pow(sin_theta,2.0);
   /* double r2    = pow(r,2.0); */
   /* a2    = pow(a,2.0); */
   /*  */
   /* Delta = r2 - 2.0*M*r + a2; */
   /* rho2  = r2 + a2*cos2; */
   /* Sigma = pow(r2 + a2,2.0) - Delta*a2*sin2; */

   #if POLAR == FALSE

   der->dlapse[0] = der_r_alpha_vals[ index ];
   der->dlapse[1] = 0.0;
   der->dlapse[2] = 0.0;

   der->dbeta[0][0] = der_r_beta_con_vals[ index ];
   der->dbeta[0][1] = 0.0;
   der->dbeta[0][2] = 0.0;
   der->dbeta[1][0] = 0.0;
   der->dbeta[1][1] = 0.0;
   der->dbeta[1][2] = 0.0;
   der->dbeta[2][0] = 0.0;
   der->dbeta[2][1] = 0.0;
   der->dbeta[2][2] = 0.0;

   der->dgam[0][0][0] = der_r_gamma_rr_cov_vals[ index ];
   der->dgam[0][0][1] = 0.0;
   der->dgam[0][0][2] = 0.0;
   der->dgam[0][1][0] = 0.0;
   der->dgam[0][1][1] = 2.0 * r;
   der->dgam[0][1][2] = 0.0;
   der->dgam[0][2][0] = 0.0;
   der->dgam[0][2][1] = 0.0;
   der->dgam[0][2][2] = 2.0 * r * sin_theta*sin_theta;

   der->dgam[1][0][0] = 0.0;
   der->dgam[1][0][1] = 0.0;
   der->dgam[1][0][2] = 0.0;
   der->dgam[1][1][0] = 0.0;
   der->dgam[1][1][1] = 0.0;
   der->dgam[1][1][2] = 0.0;
   der->dgam[1][2][0] = 0.0;
   der->dgam[1][2][1] = 0.0;
   der->dgam[1][2][2] = r * r * sin_theta * cos_theta;

   der->dgam[2][0][0] = 0.0;
   der->dgam[2][0][1] = 0.0;
   der->dgam[2][0][2] = 0.0;
   der->dgam[2][1][0] = 0.0;
   der->dgam[2][1][1] = 0.0;
   der->dgam[2][1][2] = 0.0;
   der->dgam[2][2][0] = 0.0;
   der->dgam[2][2][1] = 0.0;
   der->dgam[2][2][2] = 0.0;

   #elif POLAR == TRUE

   /* printf("The metric is not defined in polar coordinates\n"); */
   /* exit(EXIT_FAILURE); */

   der->dlapse[0] = der_r_alpha_vals[ index ];
   der->dlapse[1] = 0.0;
   der->dlapse[2] = 0.0;

   der->dbeta[0][0] = der_r_beta_con_vals[ index ];
   der->dbeta[0][1] = 0.0;
   der->dbeta[0][2] = 0.0;
   der->dbeta[1][0] = 0.0;
   der->dbeta[1][1] = 0.0;
   der->dbeta[1][2] = 0.0;
   der->dbeta[2][0] = 0.0;
   der->dbeta[2][1] = 0.0;
   der->dbeta[2][2] = 0.0;

   der->dgam[0][0][0] = der_r_gamma_rr_cov_vals[ index ];
   der->dgam[0][0][1] = 0.0;
   der->dgam[0][0][2] = 0.0;
   der->dgam[0][1][0] = 0.0;
   der->dgam[0][1][1] = 2.0 * r * sin2;
   der->dgam[0][1][2] = 0.0;
   der->dgam[0][2][0] = 0.0;
   der->dgam[0][2][1] = 0.0;
   der->dgam[0][2][2] = 2.0 * r;

   der->dgam[1][0][0] = 0.0;
   der->dgam[1][0][1] = 0.0;
   der->dgam[1][0][2] = 0.0;
   der->dgam[1][1][0] = 0.0;
   der->dgam[1][1][1] = 0.0;
   der->dgam[1][1][2] = 0.0;
   der->dgam[1][2][0] = 0.0;
   der->dgam[1][2][1] = 0.0;
   der->dgam[1][2][2] = 0.0;

   der->dgam[2][0][0] = 0.0;
   der->dgam[2][0][1] = 0.0;
   der->dgam[2][0][2] = 0.0;
   der->dgam[2][1][0] = 0.0;
   der->dgam[2][1][1] = 2.0 * r*r * cos_theta * sin_theta;
   der->dgam[2][1][2] = 0.0;
   der->dgam[2][2][0] = 0.0;
   der->dgam[2][2][1] = 0.0;
   der->dgam[2][2][2] = 0.0;

   #endif

#endif
}

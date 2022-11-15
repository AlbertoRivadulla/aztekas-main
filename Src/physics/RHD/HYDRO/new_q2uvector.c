/**
 * @file /RHD/q2uvector.c
 *
 * @author Alejandro Aguayo-Ortiz
 *
 * @brief Function that convert from Conservative to Primitives (RHD).
 *
 */

#include"main.h"
    
int Cons2Prim(double *u, double *q)
{
   int i, j, k;
   int count;
   double D, tau, S_cov[3], S_con[3];
   double rho, p;
   double h, derh, f, derf;
   double F, dF;
   double Lorentz, derLorentz, SS;
   double theta, theta_0;
   double x, x_0, y, y_0;
   gauge_ local_grid;
   
#if DIM == 1

   for(i = gc; i <= Nx1-0; i++)
   {
      local_grid.x[0] = grid.time;
      local_grid.x[1] = grid.X1[i];
      local_grid.x[2] = 0.0;
      local_grid.x[3] = 0.0;
      #if COORDINATES == SPHERICAL
      local_grid.x[2] = M_PI_2;
      #endif

      Get_Metric_Components(&local_grid);

      D        = q(0,i);
      tau      = q(1,i);
      S_cov[0] = q(2,i);
      S_cov[1] = 0.0;
      S_cov[2] = 0.0;

      S_con[0] = local_grid.gamma_con[0][0]*S_cov[0] + \
                 local_grid.gamma_con[0][1]*S_cov[1] + \
                 local_grid.gamma_con[0][2]*S_cov[2];
      S_con[1] = local_grid.gamma_con[1][0]*S_cov[0] + \
                 local_grid.gamma_con[1][1]*S_cov[1] + \
                 local_grid.gamma_con[1][2]*S_cov[2];
      S_con[2] = local_grid.gamma_con[2][0]*S_cov[0] + \
                 local_grid.gamma_con[2][1]*S_cov[1] + \
                 local_grid.gamma_con[2][2]*S_cov[2];
      
      SS = S_cov[0]*S_con[0] + S_cov[1]*S_con[1] + S_cov[2]*S_con[2];

      h = 1.0 + U(PRE,i)*K/(U(RHO,i)*(K - 1.0));
      Lorentz = sqrt(1.0 + SS/(D*D*h*h));
      x_0 = D*h*Lorentz;

      F  = 1.0;
      dF = 1.0;

      count = 0;
   
      while(F > 0.000000001)
      {
         Lorentz    = 1.0/sqrt(1.0 - SS/(x_0));
         derLorentz = - SS*pow(Lorentz/x,3.0);

         #if EOS == IDEAL
         F  = x_0 - (K - 1.0)*(x_0 - Lorentz*D)/(K*Lorentz*Lorentz) - tau - D; 
         dF = 1.0 - (K - 1.0)*((1.0 - derLorentz*D)*Lorentz*Lorentz - 2.0*Lorentz*derLorentz*(x_0 - Lorentz*D))/(K*pow(Lorentz,4.0));
         #endif

         x = x_0 - F/dF;

         x_0 = x;

         count++;

         printf("%e %e %e %e %e %e\n",x,Lorentz,F,dF,grid.X1[i],h);
         getchar();

         if(count == 100000)
         {
            printf("                                          \n");
            printf("Spend too much time in Newton-Rhapson.\n");
            CHECK_NAN = TRUE;
            U = U0;
            PrintValues(&grid.time,&theta,&CHECK_NAN);
            exit(EXIT_FAILURE);
         }
      }

      Lorentz = 1.0/sqrt(1.0 - SS/(x*x));

      #if EOS == IDEAL
      p    = ((K - 1.0)/K)*(x - Lorentz*D)/(Lorentz*Lorentz);
      #endif

      u(RHO,i) = D/Lorentz;
      u(PRE,i) = p;
      u(VX1,i) = S_cov[0]/x;
   }

#elif DIM == 2

   for(i = 0; i <= Nx1-0; i++)
   {
      for(j = 0; j <= Nx2-0; j++)
      {
#ifdef INTERPOLATED_METRIC
         local_grid.I[0] = i;
#endif
         local_grid.x[0] = grid.time;
         local_grid.x[1] = grid.X1[i];
         local_grid.x[2] = grid.X2[j];
         local_grid.x[3] = 0.0;
         #if POLAR == TRUE
         local_grid.x[2] = M_PI_2;
         #endif

         Get_Metric_Components(&local_grid);

         D        = q(0,i,j);
         tau      = q(1,i,j);
         S_cov[0] = q(2,i,j);
         S_cov[1] = q(3,i,j);
         S_cov[2] = 0.0;

         S_con[0] = local_grid.gamma_con[0][0]*S_cov[0] + \
                    local_grid.gamma_con[0][1]*S_cov[1] + \
                    local_grid.gamma_con[0][2]*S_cov[2];
         S_con[1] = local_grid.gamma_con[1][0]*S_cov[0] + \
                    local_grid.gamma_con[1][1]*S_cov[1] + \
                    local_grid.gamma_con[1][2]*S_cov[2];
         S_con[2] = local_grid.gamma_con[2][0]*S_cov[0] + \
                    local_grid.gamma_con[2][1]*S_cov[1] + \
                    local_grid.gamma_con[2][2]*S_cov[2];
         
         SS = S_cov[0]*S_con[0] + S_cov[1]*S_con[1] + S_cov[2]*S_con[2];

         theta_0 = U(1,i,j)/U(0,i,j);
         f       = 1.0;
         count   = 0;

         while(fabs(f) > 0.00000001)
         {
         #if EOS == IDEAL
            h    = 1.0 + (K / (K - 1.0))*theta_0;
            derh = K / (K - 1.0);
         #elif EOS == DUST
            h    = 1.0;
            derh = 0.0;
         #elif EOS == STIFF
            h    = (K / (K - 1.0))*theta_0;
            derh = K / (K - 1.0);
         #endif

            Lorentz = sqrt(1.0 + SS/(D*D*h*h));

            f    = h*Lorentz - (theta_0/Lorentz) - (tau/D) - 1.0;
            derf = (1.0/Lorentz)*(derh - 1.0 - theta_0*((Lorentz*Lorentz - 1.0)/(Lorentz*Lorentz))*(derh/h));

            theta   = theta_0 - f/derf;
            theta_0 = theta;
            count++;

            if(count == 100000)
            {
               printf("                                          \n");
               printf("Spend too much time in Newton-Rhapson.\n");
               CHECK_NAN = TRUE;
               U = U0;
               PrintValues(&grid.time,&theta,&CHECK_NAN);
               exit(EXIT_FAILURE);
            }
         }

         #if EOS == IDEAL
         h    = 1.0 + (K / (K - 1.0))*theta_0;
         #elif EOS == DUST
         h    = 1.0;
         #elif EOS == STIFF
         h    = (K / (K - 1.0))*theta_0;
         #endif

         Lorentz = sqrt(1.0 + SS/(D*D*h*h));

         u(0,i,j) = D / Lorentz;
         u(1,i,j) = D*h*Lorentz - tau - D;
         u(2,i,j) = S_cov[0]/(D*h*Lorentz);
         u(3,i,j) = S_cov[1]/(D*h*Lorentz);
      }
   }

#elif DIM == 4

   for(i = 0; i <= Nx1-0; i++)
   {
      for(j = 0; j <= Nx2-0; j++)
      {
         local_grid.x[0] = grid.time;
         local_grid.x[1] = grid.X1[i];
         local_grid.x[2] = grid.X2[j];
         local_grid.x[3] = 0.0;

         Get_Metric_Components(&local_grid);

         D        = q(0,i,j);
         tau      = q(1,i,j);
         S_cov[0] = q(2,i,j);
         S_cov[1] = q(3,i,j);
         S_cov[2] = q(4,i,j);

         S_con[0] = local_grid.gamma_con[0][0]*S_cov[0] + \
                    local_grid.gamma_con[0][1]*S_cov[1] + \
                    local_grid.gamma_con[0][2]*S_cov[2];
         S_con[1] = local_grid.gamma_con[1][0]*S_cov[0] + \
                    local_grid.gamma_con[1][1]*S_cov[1] + \
                    local_grid.gamma_con[1][2]*S_cov[2];
         S_con[2] = local_grid.gamma_con[2][0]*S_cov[0] + \
                    local_grid.gamma_con[2][1]*S_cov[1] + \
                    local_grid.gamma_con[2][2]*S_cov[2];
         
         SS = S_cov[0]*S_con[0] + S_cov[1]*S_con[1] + S_cov[2]*S_con[2];

         theta_0 = U(1,i,j)/U(0,i,j);
         f       = 1.0;
         count   = 0;

         while(fabs(f) > 0.00000001)
         {
         #if EOS == IDEAL
            h    = 1.0 + (K / (K - 1.0))*theta_0;
            derh = K / (K - 1.0);
         #elif EOS == DUST
            h    = 1.0;
            derh = 0.0;
         #elif EOS == STIFF
            h    = (K / (K - 1.0))*theta_0;
            derh = K / (K - 1.0);
         #endif

            Lorentz = sqrt(1.0 + SS/(D*D*h*h));

            f    = h*Lorentz - (theta_0/Lorentz) - (tau/D) - 1.0;
            derf = (1.0/Lorentz)*(derh - 1.0 - theta_0*((Lorentz*Lorentz - 1.0)/(Lorentz*Lorentz))*(derh/h));

            theta   = theta_0 - f/derf;
            theta_0 = theta;
            count++;

            if(count == 10000000)
            {
               printf("                                          \n");
               printf("Spend too much time in Newton-Rhapson.\n");
               printf("%e %e %e %e %e %e\n",f,theta,SS,Lorentz,grid.X1[i],grid.X2[j]);
               CHECK_NAN = TRUE;
               U = U0;
               PrintValues(&grid.time,&theta,&CHECK_NAN);
               exit(EXIT_FAILURE);
            }
         }

         #if EOS == IDEAL
         h    = 1.0 + (K / (K - 1.0))*theta_0;
         #elif EOS == DUST
         h    = 1.0;
         #elif EOS == STIFF
         h    = (K / (K - 1.0))*theta_0;
         #endif

         Lorentz = sqrt(1.0 + SS/(D*D*h*h));

         u(0,i,j) = D / Lorentz;
         u(1,i,j) = D*h*Lorentz - tau - D;
         u(2,i,j) = S_cov[0]/(D*h*Lorentz);
         u(3,i,j) = S_cov[1]/(D*h*Lorentz);
         u(4,i,j) = S_cov[2]/(D*h*Lorentz);
      }
   }

#elif DIM == 3

   for(i = 0; i <= Nx1; i++)
   {
      for(j = 0; j <= Nx2; j++)
      {
         for(k = 0; k <= Nx3; k++)
         {
            D  = q(0,i,j,k);
            E  = q(1,i,j,k);
            S1 = q(2,i,j,k);
            S2 = q(3,i,j,k);
            S3 = q(4,i,j,k);
 
            u(0,i,j,k) = D;
            #if EOS == IDEAL
            u(1,i,j,k) = ((2.0*K-2.0)*D*E+(1.0-K)*pow(S3,2.0)+(1.0-K)*pow(S2,2.0)+(1.0-K)*pow(S1,2.0))/(2.0*D);
            #elif EOS == DUST
            u(1,i,j,k) = 0.0;
            #endif
            u(2,i,j,k) = S1/D;
            u(3,i,j,k) = S2/D;
            u(4,i,j,k) = S3/D;
         }
      }
   }

#endif
   
   return 0;
}

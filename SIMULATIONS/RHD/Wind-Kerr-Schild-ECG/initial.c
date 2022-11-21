/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//Do not erase any of these libraries//
#include"main.h"

void Initial()
{
   int n, i, j, k, cell;

   //Initialize grid.time
   grid.time = 0.0;

   //Initialize dt
   dt = 0.0;

   gauge_ local_grid;
   velocity_0 = vinf;

   // Load the ECG metric
   Read_Interpolation_f_Metric();

   // Compute the values of f(r) and f'(r) in the grid
   Compute_f_In_Grid();

   // Set the program to end when a stationary state is reached for the accretion rate
   Mdot_end = FALSE;
   /* Mdot_end = 0; */

#if DIM == 2

   ////////////////////////////
   //----------Wind----------//
   ////////////////////////////

   for(i = 0; i <= Nx1; i++)
   {
      for(j = 0; j <= Nx2; j++)
      {
#ifdef INTERPOLATED_METRIC
         local_grid.I[0] = i;
#endif
         local_grid.x[1] = grid.X1[i];
#if POLAR == FALSE
         local_grid.x[2] = grid.X2[j];
#elif POLAR == TRUE
         local_grid.x[2] = M_PI_2;
#endif

         double r = grid.X1[i];
         double p = grid.X2[j];
         double t = M_PI_2;
         double M = Black_Hole_Mass;
         double a = Black_Hole_Spin;

         Get_Metric_Components(&local_grid);

         double rho2 = r*r + a*a*cos(t)*cos(t);
         
         double gamrr = 1.0 + 2.0*M*r/rho2;
         double gamrp = -a*(gamrr)*sin(t)*sin(t);
         double gampp = sin(t)*sin(t)*(rho2 + a*a*(gamrr)*sin(t)*sin(t));

         double F1 = 1.0/sqrt(gamrr);
         double F4 = -2.0*gamrp/(sqrt(gamrr)*gampp);
         double F3 = (F1*gamrr + F4*gamrp)/sqrt((gamrr*gampp - -gamrp*gamrp));
         double F2 = (F4*gampp + F1*gamrp)/sqrt((gamrr*gampp - -gamrp*gamrp));

         double vr =  F1*velocity_0*cos(grid.X2[j]) + F2*velocity_0*sin(grid.X2[j]);
         double vp = -F3*velocity_0*sin(grid.X2[j]) + F4*velocity_0*cos(grid.X2[j]);

         U(0,i,j) =  density_0;
         U(1,i,j) =  (K - 1.0)*U(0,i,j)*pow(velocity_0/Mach,2.0)/(K*(K - 1.0) - K*pow(velocity_0/Mach,2.0));
         U(2,i,j) = gamrr*vr + gamrp*vp; 
         U(3,i,j) = gamrp*vr + gampp*vp;

         /* U(0,i,j) =  density_0; */
         /* U(1,i,j) =  (K - 1.0)*U(0,i,j)*pow(velocity_0/Mach,2.0)/(K*(K - 1.0) - K*pow(velocity_0/Mach,2.0)); */
         /* U(2,i,j) =  velocity_0*cos(grid.X2[j])/sqrt(local_grid.gamma_con[0][0]); */
         /* U(3,i,j) = -velocity_0*sin(grid.X2[j])/sqrt(local_grid.gamma_con[1][1]); */
      }
   }

#endif
}

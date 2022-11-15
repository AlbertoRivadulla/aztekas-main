/**
 * @file /common/print_time_values.c
 *
 * @authors Alejandro Aguayo-Oritz and Emilio Tejeda
 *
 * @brief Print values each time.
 */

#include"main.h"

void Print_Time_Values(double *tprint, double *dtprint, int *itprint)
{
   /**
    * Print the Mass Accretion Rate \f[ \dot{M} \f]
    */
   #if ANALYSIS == TRUE
   Analysis(U);
   #endif

   #if MDOT == TRUE
   Mass_Accretion_Rate(U);
   #endif

   if(grid.time >= *tprint || CHECK_NAN == TRUE)
   {
      /* printf("Time = %e, dt = %e\n",grid.time,dt); */

      int time_sec;
      int hr, min, sec;
      #ifdef _OPENMP
         time_sec = (int)(omp_get_wtime() - start);
         hr       = time_sec/3600;
         min      = (time_sec%3600)/60;
         sec      = (time_sec%60)%60;
      #else
         time_sec = (int)((double)(clock()-start)/CLOCKS_PER_SEC);
         hr       = time_sec/3600;
         min      = (time_sec%3600)/60;
         sec      = (time_sec%60)%60;
      #endif
      printf("Time = %e, dt = %e, Running time = %d hr : %d min : %d sec\n",\
            grid.time,dt,hr,min,sec);

      if(binary == TRUE)
      {
         Output_bin(itprint);
      }
      else
      {
         Output_ascii(itprint);
      }

      /**
       * Increase the time of printing by dtprint and increase
       * the humber of the output file
       */
      *tprint = *tprint + *dtprint;
      ++*itprint;

      /**
       * Forcing termination
       */
      Alternative_Termination();
   }
}

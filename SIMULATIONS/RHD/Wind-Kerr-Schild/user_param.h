#include"macros.h"

#define POLAR              TRUE

#define outflow_x1max      TRUE
#define outflow_x1min      TRUE
#if POLAR == FALSE
   #define reflective_x2max   TRUE
   #define reflective_x2min   TRUE
#elif POLAR == TRUE
   #define periodic_x2        TRUE
#endif

#define RECONST            MC
#define FLUX               HLL
#define GRID               LOGMESH
#define lfac               1.0
#define PRINT_EVOLV        TRUE

#define Black_Hole_Mass    1.0
#define Black_Hole_Spin    0.99
#define Mach               5.0
#define vinf               0.5

double density_0, pressure_0, velocity_0;


//-----------------------------------------------------------------------------
//    Variables and functions for computing the mass accretion rate
//-----------------------------------------------------------------------------

#define ANALYSIS                FALSE
#define MDOT                    TRUE
// #define MDOT_END                TRUE
#define MDOT_END                FALSE
#define MDOT_DATA               1.0e+04
int Mdot_end;
double Mdot_0;
double Mdot_tprint;

void Mass_Accretion_Rate(double *B);
double rs(double K, double c_s);

double Mdot_Mean;
double cs, Temp;
double rB, tB;
char last[50];

int count;
int plus;
int minus;
int restart_file;
double Mdot_Max;
double Mdot_Min;
double MDOT_ERR;
double MDOT_TIME;
char eqstate[50];

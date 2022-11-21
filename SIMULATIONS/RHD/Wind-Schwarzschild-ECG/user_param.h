#include"macros.h"

// Define boundaries TRUE = 1 , FALSE = 0
// Set as cond_X whereas
// cond : {outflow,reflective,periodic,inflow}
// X : {x1max,x1min,x2max,x2min,x3max,x3min}

#define POLAR              FALSE

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
// Form of the grid: UNIFORM, LOGMESH, TORTOISE
// lfac is a parameter for the logarithmic mesh
#define GRID               LOGMESH
#define lfac               1.0
// Set this to false to avoid printing to screen after each step of the integration,
// and print only when a new output file has been generated
// #define PRINT_EVOLV        TRUE
#define PRINT_EVOLV        FALSE

#define Black_Hole_Mass    1.0
#define Black_Hole_Spin    0.0
#define Mach               5.0
#define vinf               0.5

double density_0, pressure_0, velocity_0;


//-----------------------------------------------------------------------------
//    Variables and functions for computing the mass accretion rate
//-----------------------------------------------------------------------------

#define ANALYSIS                FALSE
#define MDOT                    TRUE
// #define MDOT_END                TRUE
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


//-----------------------------------------------------------------------------
//    Variables and functions for the ECG black hole
//-----------------------------------------------------------------------------

#define INTERPOLATED_METRIC

// Function to clear the resources of the interpolated metric
void Free_Interpolated_Metric();

double epsilon_cubic;

// Struct for the data of an interpolation, constructed with a piecewise polynomial
struct Interpolating_Piecewise_Poly
{
    double xmin;
    double xmax;
    int n_points;

    double* coeffs;
    double* limits;
};

// Global variable with the data of an interpolation for the function f(r) in the metric
struct Interpolating_Piecewise_Poly interp_f_ECG;

// Arrays with the values of f(r) and f'(r) for each value of r in the grid
double* f_vals;
double* f_prime_vals;

// Function to read the data for the interpolation
void Read_Interpolation_f_Metric();

// Function to compute the values of f(r) and f'(r) in the grid
void Compute_f_In_Grid();

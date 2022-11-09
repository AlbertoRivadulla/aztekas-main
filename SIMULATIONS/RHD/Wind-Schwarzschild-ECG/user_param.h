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
#define PRINT_EVOLV        TRUE

#define Black_Hole_Mass    1.0
#define Black_Hole_Spin    0.0
#define Mach               5.0
#define vinf               0.5

double density_0, pressure_0, velocity_0;

//-----------------------------------------------------------------------------
//    Variables and functions for the ECG black hole
//-----------------------------------------------------------------------------

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

// Function to read the data for the interpolation
void Read_Interpolation_f_Metric();

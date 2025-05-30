/* File:     mpi_trap4.c
 * Purpose:  Use MPI to implement a parallel version of the trapezoidal 
 *           rule.  This version uses collective communications and
 *           MPI derived datatypes to distribute the input data and 
 *           compute the global sum.
 *
 * Input:    The endpoints of the interval of integration and the number
 *           of trapezoids
 * Output:   Estimate of the integral from a to b of f(x)
 *           using the trapezoidal rule and n trapezoids.
 *
 * Compile:  mpicc -g -Wall -o mpi_trap4 mpi_trap4.c
 * Run:      mpiexec -n <number of processes> ./mpi_trap4
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of
 *        integration.
 *    2.  Each process estimates the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3a. Each process != 0 sends its integral to 0.
 *    3b. Process 0 sums the calculations received from
 *        the individual processes and prints the result.
 *
 * Note:  f(x) is all hardwired.
 *
 * IPP:   Section 3.5 (pp. 117 and ff.)
 */
#include <stdio.h>
#include <math.h>
#include <mpi.h>  //We'll be using MPI routines, definitions, etc.

#define TSCALE 286.478897565412000
#define ASCALE 0.236589076381454
#define VSCALE 67.777777777777900

/* Build a derived datatype for distributing the input data */
void Build_mpi_type(double* a_p, double* b_p, int* n_p, MPI_Datatype* input_mpi_t_p);
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p); //Get the input values

double doubleTrap(double left_endpt, double right_endpt, int trap_count, double base_len);    
float f(float x); 

//new ones
float leftRiemann(float left_endpt, float right_endpt, int trap_count, float base_len);
double midRiemann(double left_endpt, double right_endpt, int trap_count, double base_len);  
float floatTrap(float left_endpt, float right_endpt, int trap_count, float base_len);


int main(void) {
   int my_rank, comm_sz, n, local_n;   
   double a, b, h, local_a, local_b;
   double local_int, total_int;

   MPI_Init(NULL, NULL);   //Let the system do what it needs to start up MPI

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  //Get my process rank

   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);  //Find out how many processes are being used

   Get_input(my_rank, comm_sz, &a, &b, &n);

   h = (b-a)/n;          /* h is the same for all processes */
   local_n = n/comm_sz;  /* So is the number of trapezoids  */

   /* Length of each process' interval of
    * integration = local_n*h.  So my interval
    * starts at: */
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;

   
   //local_int = leftRiemann(local_a, local_b, local_n, h);
   //local_int = midRiemann(local_a, local_b, local_n, h);
   local_int = floatTrap(local_a, local_b, local_n, h);
   //local_int = doubleTrap(local_a, local_b, local_n, h);


   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);   //Add up the integrals calculated by each process

   if(my_rank == 0){ //Print the result
      printf("With n = %d rectangles, our estimate\n", n);
      printf("of the integral from %f to %f = %20.15f\n",
          a, b, total_int);
   }

   MPI_Finalize();   //Shut down MPI

   return 0;
} /*  main  */

/*------------------------------------------------------------------
 * Function:     Build_mpi_type
 * Purpose:      Build a derived datatype so that the three
 *               input values can be sent in a single message.
 * Input args:   a_p:  pointer to left endpoint
 *               b_p:  pointer to right endpoint
 *               n_p:  pointer to number of trapezoids
 * Output args:  input_mpi_t_p:  the new MPI datatype
 */
void Build_mpi_type(
      double*        a_p            /* in  */, 
      double*        b_p            /* in  */, 
      int*           n_p            /* in  */,
      MPI_Datatype*  input_mpi_t_p  /* out */) {

   int array_of_blocklengths[3] = {1, 1, 1};
   MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
   MPI_Aint a_addr, b_addr, n_addr;
   MPI_Aint array_of_displacements[3] = {0};

   MPI_Get_address(a_p, &a_addr);
   MPI_Get_address(b_p, &b_addr);
   MPI_Get_address(n_p, &n_addr);
   array_of_displacements[1] = b_addr-a_addr; 
   array_of_displacements[2] = n_addr-a_addr; 
   MPI_Type_create_struct(3, array_of_blocklengths, 
         array_of_displacements, array_of_types,
         input_mpi_t_p);
   MPI_Type_commit(input_mpi_t_p);
}  /* Build_mpi_type */

/*------------------------------------------------------------------
 * Function:     Get_input
 * Purpose:      Get the user input:  the left and right endpoints
 *               and the number of trapezoids
 * Input args:   my_rank:  process rank in MPI_COMM_WORLD
 *               comm_sz:  number of processes in MPI_COMM_WORLD
 * Output args:  a_p:  pointer to left endpoint               
 *               b_p:  pointer to right endpoint               
 *               n_p:  pointer to number of trapezoids
 */
void Get_input(
      int      my_rank  /* in  */, 
      int      comm_sz  /* in  */, 
      double*  a_p      /* out */, 
      double*  b_p      /* out */,
      int*     n_p      /* out */) {
   MPI_Datatype input_mpi_t;
   int rc=0;

   Build_mpi_type(a_p, b_p, n_p, &input_mpi_t);

   if (my_rank == 0) {
      printf("Enter a, b, and n\n");
      rc=scanf("%lf %lf %d", a_p, b_p, n_p); if(rc < 0) perror("Get_input");
   } 
   MPI_Bcast(a_p, 1, input_mpi_t, 0, MPI_COMM_WORLD);

   MPI_Type_free(&input_mpi_t);
}  /* Get_input */

/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Serial function for estimating a definite integral 
 *               using the trapezoidal rule
 * Input args:   left_endpt
 *               right_endpt
 *               trap_count 
 *               base_len
 * Return val:   Trapezoidal rule estimate of integral from
 *               left_endpt to right_endpt using trap_count
 *               trapezoids
 */
double doubleTrap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
} /*  Trap  */



float leftRiemann(float left_endpt, float right_endpt, int trap_count, float base_len){
   float sum = 0.0f;
   for(int i = 0; i < trap_count; i++){
       float x = left_endpt + i * base_len;
       sum += f(x);
   }
   return sum * base_len;
}


double midRiemann(double left_endpt, double right_endpt, int trap_count, double base_len){
   double sum = 0.0;
   for(int i = 0; i < trap_count; i++){
       double x = left_endpt + (i + 0.5) * base_len;
       sum += f(x);
   }
   return sum * base_len;
}


float floatTrap(
   float left_endpt  /* in */, 
   float right_endpt /* in */, 
   int    trap_count  /* in */, 
   float base_len    /* in */) {
float estimate, x; 
int i;

estimate = (f(left_endpt) + f(right_endpt))/2.0;
for (i = 1; i <= trap_count-1; i++) {
   x = left_endpt + i*base_len;
   estimate += f(x);
}
estimate = estimate*base_len;

return estimate;
}


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
float f(float x /* in */) {
   //return x*x;
   //return sin(x);
   
   //return ((sin(x/TSCALE)) * ASCALE);
   return ((-cos(x/TSCALE)+1) * VSCALE);//antiderivative of ((sin(x/TSCALE)) * ASCALE)

   // replace this with linear interpolation of any function such as acceleration or velocity from a model
   //return table_interp(x);
} /* f */



//#include "ex4accel.h"
#include "ex4vel.h"
#include <stdlib.h>

// table look-up for function profile given and velocity profile determined
double table_function(int timeidx)
{
    long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

    // Check array bounds for look-up table
    if(timeidx > tsize)
    {
        printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize-1);
        exit(-1);
    }

    return DefaultProfile[timeidx];
}


double table_interp(double time)
{
    int timeidx = (int)time;
    int timeidx_next = ((int)time)+1;
    double delta_t = time - (double)((int)time);

    return ( table_function(timeidx) + ( (table_function(timeidx_next) - table_function(timeidx)) * delta_t));

}
 
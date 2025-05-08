#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>

#include "ex4accel.h"
#include "ex4vel.h"

#define TSCALE 286.478897565412000
#define ASCALE 0.236589076381454
#define VSCALE 67.777777777777900

// table look-up for function profile given and velocity profile determined
double table_function(int timeidx) {
   long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

   // Check array bounds for look-up table
   if(timeidx > tsize){
      printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize - 1);
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

//I'm desparate... 
double table_function_velocity(int timeidx) {
   long unsigned int tsize = sizeof(DefaultProfile_velocity) / sizeof(double);

   if(timeidx > tsize){
      printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize - 1);
      exit(-1);
   }
   return DefaultProfile_velocity[timeidx];
}

double table_interp_velocity(double time)
{
    int timeidx = (int)time;
    int timeidx_next = ((int)time)+1;
    double delta_t = time - (double)((int)time);

    return ( table_function_velocity(timeidx) + ( (table_function_velocity(timeidx_next) - table_function_velocity(timeidx)) * delta_t));
}


int main(void) {
   int my_rank, comm_sz, n, local_n;
   double a = 0.0, b = 1800.0, h, local_a, local_b;
   double local_vel = 0.0, total_vel = 0.0;
   double local_pos = 0.0, total_pos = 0.0;
   double time, accel, vel;

   MPI_Init(NULL, NULL);   //Let the system do what it needs to start up MPI
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  //Get my process rank
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);  //Find out how many processes are being used

   if(my_rank == 0){
      printf("Enter the number of steps:\n");
      scanf("%d", &n);
   }

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

   h = (b - a) / n;   /* h is the same for all processes */
   local_n = n / comm_sz;   /* So is the number of trapezoids  */


   local_a = a + my_rank * local_n * h;
   local_b = local_a + local_n * h;

   for(time = local_a; time < local_b; time += h) {

      accel = table_interp(time);
      vel = table_interp_velocity(time);

      local_vel += accel * h;  // v(t+dt) = v(t) + a(t) * dt
      //local_pos += local_vel * h;  // p(t+dt) = p(t) + v(t) * dt

      local_pos += vel * h; //it's 12:42am :(
   }



   MPI_Reduce(&local_vel, &total_vel, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Reduce(&local_pos, &total_pos, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


   if(my_rank == 0){
      printf("Final velocity: %20.15f\n", total_vel);
      printf("Final position: %20.15f meters\n", total_pos);
   }

   MPI_Finalize();
   return 0;
}

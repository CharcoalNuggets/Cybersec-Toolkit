//https://www.codewithc.com/c-program-for-regula-falsi-method/
//
// Refactored to improve variable names and comments
// 
// Sam Siewert, 4/26/22
//
// Regula Falsi does not require the derivative of the function to estimate x0 new interval
// that contains the root like Newton-Raphson, but rather uses the SECANT, which can simply be
// determined by evaluating the function at each end of the search interval.
//
// The intersection of the SECANT based upon slope, will ALWAYS contain the ZERO either on the 
// LEFT side of the search interval or the RIGHT side.
//
// We simply pick the side that contains the ZERO and iterate.
//
// Theory can be understood by starting with Wikipedia - https://en.wikipedia.org/wiki/Regula_falsi
//
// The double false position is the fastest converging and most reliable.
//
#include<stdio.h>
#include<math.h>
#include <time.h>


double f(double x)
{
    //return cos(x) - x*exp(x);

    // As can be seen with Desmos, this example equation has roots near: -2.851, 4.758, and 7.792
    //return (-(x*x*x) + 9.7*(x*x) -1.3*x -105.7);
    return cos(x*x);
    //return cos(x);
    //return sin(x);
    //return((x*x)+2.0*x+1.0);
    //return cos(x*x) + 1.5*x - 2.5 - sin(x*x);
}

void regula (double *x, double x0, double x1, double fx0, double fx1, int *itr)
{

    // Comput new x based on secant method rather than just stepping brute force or
    // using the slope X axis intercept, which requires df/dx.
    //
    // slope of secant is rise/run, which is [f(x1) - f(x0)] / [x1 - x0]
    //
    // The new x that intercepts the X axis, has the SAME SLOPE AS THE SECANT.
    //
    // So, [f(x0) - 0] / [x0 - x] = [f(x1) - f(x0)] / [x1 - x0]
    //
    // Or, rearranging, we get [x1 - x0] * f(x0) / [x0 - x] = f(x1) - f(x0),
    //
    // which can be further rearranged to get:
    //
    // x0 - x = [x1 - x0] * f(x0) / [f(x1 - f(x0)]
    // x = x0 - [x1 - x0] / [f(x1) - f(x0)] * f(x0)
    //
    *x = x0 - ((x1 - x0) / (fx1 - fx0))*fx0;

    ++(*itr);

    //printf("Iteration no. %3d X = %7.5f \n", *itr, *x);
}


int main(void)
{
    //int itr = 0;
    int maxmitr, total_roots = 0;
    double x0, x1, x2, x3, step, allerr;
    double start_range, end_range;
    struct timespec start, end;

    printf("\nEnter the values of x0, x1, step size, allowed error and maximum iterations:\n");
    scanf("%lf %lf %lf %lf %d", &start_range, &end_range, &step, &allerr, &maxmitr);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for(x0 = start_range; x0 < end_range; x0 += step){
        x1 = x0 + step;

        if(f(x0) * f(x1) < 0){
            
            int local_itr = 0;
            regula(&x2, x0, x1, f(x0), f(x1), &local_itr);

            do{
                if(f(x0) * f(x2) < 0){
                    x1 = x2;
                }
                else{
                    x0 = x2;
                }
                regula(&x3, x0, x1, f(x0), f(x1), &local_itr);

                if(fabs(x3 - x2) < allerr){
                    //printf("Root %d found at x = %.15lf after %d iterations\n", total_roots + 1, x3, local_itr);
                    total_roots++;
                    break;
                }

                x2 = x3;

            } 
            while(local_itr < maxmitr);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Total roots found in [%.2f, %.2f]: %d\n", start_range, end_range, total_roots);
    printf("Elapsed time: %lf seconds\n", elapsed);

    return 0;
}
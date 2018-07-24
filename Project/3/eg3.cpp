#include "stdio.h"
#include "omp.h"

#define NUM_THREADS 2
static long num_steps = 100000000;
double step;

int main()
{
    int nthreads;
    double pi = 0.0;
    step = 1.0/(double)num_steps;

    omp_set_num_threads(NUM_THREADS);

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int i, id, nthrds;
        double x, sum;

        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if(id == 0) nthreads = nthrds;

        for(i=id, sum=0.0; i<num_steps; i=i+nthreads){
            x = (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }
        #pragma omp critical
            pi += step * sum;
    }


    double end = omp_get_wtime();

    printf("pi = %f, number of threads = %d, time = %f s\n", pi, NUM_THREADS, end-start);

    return 0;
}
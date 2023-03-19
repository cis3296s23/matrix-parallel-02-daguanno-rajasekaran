#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mmult_omp.c"
//#include "mmult_mpi_omp.c"
#include "mat.c"
#include "mat.h"

int main(int argc, char *argv[]) {

    //test which approaches
    bool unoptimized = false;
    bool SIMD = true;
    bool OMP = true;
    bool MPI_OMP = false;

    for (int i = 50; i <= 2000; i = i + 50) {

        printf("Matrix size: %d\n", i);

        //initialize clock
        struct timespec tpe, tps;

        //allocate memory for target matrix calculation
        double *a = malloc(i * i * sizeof(double));
        double *b = malloc(i * i * sizeof(double));

        //generate matrices
        a = gen_matrix(i, i);
        b = gen_matrix(i, i);
        double *c_calc = malloc(i * i * sizeof(double));

        //Time Unoptimized
        if(unoptimized) {
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            printf("unoptimized delta: %f\n", deltaTime(&tps, &tpe));
        }

        //Time SIMD
        if(SIMD) {
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult_simd(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            printf("SIMD        delta: %f\n", deltaTime(&tps, &tpe));
        }

        //Time OMP
        if(OMP) {
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult_omp(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            printf("OMP         delta: %f\n", deltaTime(&tps, &tpe));
        }

        //Time MPI OMP
        if(MPI_OMP) {
            clock_gettime(CLOCK_REALTIME, &tps);
            //mmult_omp_mpi(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            printf("OMP MPI      delta: %f\n", deltaTime(&tps, &tpe));
        }

        // compare each matrix without timer
        //compare_matrices(c_actual, mmultTarget, i, i);
        //compare_matrices(c_actual, mmult_simdTarget, i, i);
        //compare_matrices(*c_actual, mmult_ompTarget, i, i);
        //compare_matrices(*c_actual, mmult_mpi_ompTarget, i, i); 

        //free all memory
        free(a);
        free(b);
        free(c_calc);
    }
}
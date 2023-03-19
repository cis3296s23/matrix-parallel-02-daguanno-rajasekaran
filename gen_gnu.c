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

    printf("generating gnuplot data file . . .\n");

    FILE * fp;
    fp = fopen ("data.txt", "w+");

    if(unoptimized) {
        for (int i = 0; i <= 2000; i = i + 50) {
            fprintf(fp, "%d ", i);
            
            //initialize timespec
            struct timespec tpe, tps;

            //allocate memory for target matrix calculation
            double *a = malloc(i * i * sizeof(double));
            double *b = malloc(i * i * sizeof(double));

            //generate matrices
            a = gen_matrix(i, i);
            b = gen_matrix(i, i);
            double *c_calc = malloc(i * i * sizeof(double));

            //time matrix multiplication
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    fprintf(fp, "\n\n");

    if(SIMD) {
        for (int i = 0; i <= 2000; i = i + 50) {
            fprintf(fp, "%d ", i);
            
            //initialize timespec
            struct timespec tpe, tps;

            //allocate memory for target matrix calculation
            double *a = malloc(i * i * sizeof(double));
            double *b = malloc(i * i * sizeof(double));

            //generate matrices
            a = gen_matrix(i, i);
            b = gen_matrix(i, i);
            double *c_calc = malloc(i * i * sizeof(double));

            //time matrix multiplication
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult_simd(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    fprintf(fp, "\n\n");

    if(OMP) {
        for (int i = 0; i <= 2000; i = i + 50) {
            fprintf(fp, "%d ", i);
            
            //initialize timespec
            struct timespec tpe, tps;

            //allocate memory for target matrix calculation
            double *a = malloc(i * i * sizeof(double));
            double *b = malloc(i * i * sizeof(double));

            //generate matrices
            a = gen_matrix(i, i);
            b = gen_matrix(i, i);
            double *c_calc = malloc(i * i * sizeof(double));

            //time matrix multiplication
            clock_gettime(CLOCK_REALTIME, &tps);
            mmult_omp(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    fprintf(fp, "\n\n");

    if(MPI_OMP) {
        for (int i = 0; i <= 2000; i = i + 50) {
            fprintf(fp, "%d ", i);
            
            //initialize timespec
            struct timespec tpe, tps;

            //allocate memory for target matrix calculation
            double *a = malloc(i * i * sizeof(double));
            double *b = malloc(i * i * sizeof(double));

            //generate matrices
            a = gen_matrix(i, i);
            b = gen_matrix(i, i);
            double *c_calc = malloc(i * i * sizeof(double));

            //time matrix multiplication
            clock_gettime(CLOCK_REALTIME, &tps);
            //mmult_omp_mpi(c_calc, a, i, i, b, i, i);
            clock_gettime(CLOCK_REALTIME, &tpe);
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    printf("gnuplot data file complete\n");
}
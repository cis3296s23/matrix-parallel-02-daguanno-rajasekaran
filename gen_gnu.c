#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mmult_omp.c"
#include "mat.c"
#include "mat.h"

#define CLOCK_REALTIME 0
#define _POSIX_C_SOURCE 199309L

int main(int argc, char *argv[]) {

    //test which approaches
    bool unoptimized = false;
    bool SIMD = true;
    bool OMP = true;

    int i = 0;

    printf("generating gnuplot data file . . .\n");

    FILE * fp;
    fp = fopen("data.txt", "w+");

    printf("file opened\n");

    if(unoptimized) {
        for (i = 0; i <= 6000; i = i + 60) {
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
    } else { fprintf(fp, "0 0");}

    fprintf(fp, "\n\n");

    printf("SIMD ->\n");

    if(SIMD) {
        for (i = 0; i <= 3000; i = i + 60) {
            printf("Calculating SIMD size: %d", i);
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
            printf(", time = %f \n", deltaTime(&tps, &tpe));
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    fprintf(fp, "\n\n");

    printf("OMP ->\n");

    if(OMP) {
        for (i = 0; i <= 3000; i = i + 60) {
            printf("Calculating OMP size: %d", i);
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
            printf(", time = %f \n", deltaTime(&tps, &tpe));
            fprintf(fp, "%f\n", deltaTime(&tps, &tpe));

            free(a);
            free(b);
            free(c_calc);
        }
    }

    fprintf(fp, "\n\n");

    fclose(fp);

    printf("gnuplot data file complete\n");
}
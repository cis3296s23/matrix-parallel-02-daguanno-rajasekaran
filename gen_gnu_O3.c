#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mat.c"
#include "mat.h"

int main(int argc, char *argv[]) {

    printf("generating gnuplot O3 data file . . .\n");

    FILE * fp;
    fp = fopen ("data.txt", "w");

    printf("SIMD '-O3' ->")

    for (int i = 0; i <= 3000; i = i + 60) {
        printf("Calculating SIMD -O3 size: %d", i);
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

    fprintf(fp, "\n\n");

    fclose(fp);

    printf("gnuplot O3 data file complete\n");
}
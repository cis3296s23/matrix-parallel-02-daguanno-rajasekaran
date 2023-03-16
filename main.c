#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mmult_omp.c"
//#include "mmult_mpi_omp.c"
#include "mat.c"
#include "mat.h"

int main(int argc, char *argv[]) {

    int matrixSize = 0;
    
    int i = 0;
    for (i; i < 2000; i = i + 50) {

        matrixSize = i;
        printf("Matrix size: %d\n", matrixSize);

        //initialize clock
        clock_t start, end;
        int totalTime = 0;

        //allocate memory for target matrix calculation
        double *a = malloc(matrixSize * matrixSize * sizeof(double));
        double *b = malloc(matrixSize * matrixSize * sizeof(double));

        //generate matrices
        a = gen_matrix(matrixSize, matrixSize);
        b = gen_matrix(matrixSize, matrixSize);
        double *c_calc = malloc(matrixSize * matrixSize * sizeof(double));

        //time each matrix multiplication method

        //Unoptimized
        // start = clock();
        // mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        // end = clock();
        // totalTime = (int)(end - start);
        // printf("Total time for mmult: %d\n", totalTime);

        //SIMD
        start = clock();
        mmult_simd(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        end = clock();
        totalTime = (int)(end - start);
        printf("Total time for mmult_simd: %d\n", totalTime);

        //OMP
        start = clock();
        mmult_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        end = clock();
        totalTime = (int)(end - start);
        printf("Total time for mmult_omp: %d\n", totalTime);

        /*
        start = clock();
        mmult_mpi_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        end = clock();
        totalTime = (double)(end - start);
        printf("Total time for mmult_simd: %f\n", totalTime);
        */

        // compare each matrix without timer
        //compare_matrices(c_actual, mmultTarget, matrixSize, matrixSize);
        //compare_matrices(c_actual, mmult_simdTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, mmult_ompTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, mmult_mpi_ompTarget, matrixSize, matrixSize); 

        //free all memory
        free(a);
        free(b);
        free(c_calc);
    }
}
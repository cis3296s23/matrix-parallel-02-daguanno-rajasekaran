#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mat.c"
//#include "mmult_omp.c"
//#include "mmult_mpi_omp.c"
#include "mat.h"

int main(int argc, char *argv[]) {
    //int numTests = 1;

    srand(time(NULL));
    int matrixSize = (rand() % 100) + 5;

    printf("matrix size: %d\n", matrixSize);
    
    //for (int i = 0; i < numTests; i++) {
        //initialize clock
        clock_t start, end;
        double totalTime = 0;

        //allocate memory for target matrix calculation
        double *a = malloc(matrixSize * matrixSize * sizeof(double));
        double *b = malloc(matrixSize * matrixSize * sizeof(double));
        double *mmultTarget = malloc(matrixSize * matrixSize * sizeof(double));
        double *mmult_simdTarget = malloc(matrixSize * matrixSize * sizeof(double));


        //generate matrices
        *a = *gen_matrix(matrixSize, matrixSize);
        *b = *gen_matrix(matrixSize, matrixSize);
        double *c_calc = malloc(matrixSize * matrixSize * sizeof(double));

        //calculte Target with mmult
        double *c_actual = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //test each matrix with timer
        start = clock();
        mmultTarget = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        end = clock();
        totalTime = (double)(end - start);
        printf("Total time for mmult: %f\n", totalTime);

        start = clock();
        mmult_simdTarget = mmult_simd(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);
        end = clock();
        totalTime = (double)(end - start);
        printf("Total time for mmult_simd: %f\n", totalTime);

        //double *mmult_ompTarget = mmult_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //double *mmult_mpi_ompTarget = mmult_mpi_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        // compare each matrix without timer
        compare_matrices(c_actual, mmultTarget, matrixSize, matrixSize);
        compare_matrices(c_actual, mmult_simdTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, *mmult_ompTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, *mmult_mpi_ompTarget, matrixSize, matrixSize);

        //print matrix for fun
        /*
        printf("matrix a: \n");
        print_matrix(a, matrixSize, matrixSize);
        printf("matrix b: \n");
        print_matrix(b, matrixSize, matrixSize);
        printf("matrix c: \n");
        print_matrix(c_actual, matrixSize, matrixSize);
        */
        

        //free all memory
        free(a);
        free(b);
        //free(c_actual);
        //free(mmultTarget);
        //free(mmult_simdTarget);
        //free(mmult_ompTarget);
        //free(mmult_mpi_ompTarget);
    //}
}
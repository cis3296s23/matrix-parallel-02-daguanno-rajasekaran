#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmult_simd.c"
#include "mmult.c"
//#include "mmult_omp.c"
//#include "mmult_mpi_omp.c"
#include "mat.h"

int main(int argc, char *argv[]) {
    int numTests = 1;

    srand(time(NULL));
    int matrixSize = rand() % 10;
    
    //for (int i = 0; i < numTests; i++) {
        //initialize clock
        clock_t start, end;

        //allocate memory for target matrix calculation
        double *a, *b, mmultTarget, mmult_simdTarget = malloc(matrixSize * matrixSize * sizeof(double));

        //generate matrices
        *a = gen_matrix(matrixSize, matrixSize);
        *b = gen_matrix(matrixSize, matrixSize);
        double *c_calc = malloc(matrixSize * matrixSize * sizeof(double));

        //calculte Target with mmult
        double *c_actual = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //test each matrix with timer

        *mmultTarget = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        *mmult_simdTarget = mmult_simd(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //double *mmult_ompTarget = mmult_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //double *mmult_mpi_ompTarget = mmult_mpi_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        // compare each matrix without timer
        compare_matrices(*c_actual, *mmultTarget, matrixSize, matrixSize);
        compare_matrices(*c_actual, *mmult_simdTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, *mmult_ompTarget, matrixSize, matrixSize);
        //compare_matrices(*c_actual, *mmult_mpi_ompTarget, matrixSize, matrixSize);

        //print matrix for fun
        print_matrix(*a, matrixSize, matrixSize);
        print_matrix(*b, matrixSize, matrixSize);
        print_matrix(*c_actual, matrixSize, matrixSize);

        //free all memory
        free(a);
        free(b);
        free(c_actual);
        free(mmultTarget);
        free(mmult_simdTarget);
        //free(mmult_ompTarget);
        //free(mmult_mpi_ompTarget);
    //}
}
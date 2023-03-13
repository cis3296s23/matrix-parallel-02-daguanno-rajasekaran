#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mmult_simd.c"
#include "mmult.c"
#include "mmult_omp.c"
#include "mmult_mpi_omp.c"
#include "mat.h"

int main(int argc, char *argv[]) {
    int numTests = 1;

    srand(time(NULL));
    int matrixSize = rand() % 10;
    
    for (int i = 0; i < numTests; i++) {
        //initialize clock
        clock_t start, end;

        //generate matrices and allocate memory for target matrix calculation
        double *a = malloc(gen_matrix(matrixSize, matrixSize));
        double *b = malloc(gen_matrix(matrixSize, matrixSize));
        double *c_calc = malloc(matrixSize * matrixSize * sizeof(double));

        //calculte Target with mmult
        double *c_actual = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        //test each matrix with timer

        double *mmultTarget = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        double *mmult_simdTarget = mmult_simd(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        double *mmult_ompTarget = mmult_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        double *mmult_mpi_ompTarget = mmult_mpi_omp(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

        // compare each matrix without timer
        compare_matrices(*c_actual, *mmultTarget, matrixSize, matrixSize);
        compare_matrices(*c_actual, *mmult_simdTarget, matrixSize, matrixSize);
        compare_matrices(*c_actual, *mmult_ompTarget, matrixSize, matrixSize);
        compare_matrices(*c_actual, *mmult_mpi_ompTarget, matrixSize, matrixSize);

        //print matrix for fun
        print_matrix(*a);
        print_matrix(*b);
        print_matrix(*c_actual);

        //free all memory
        free(a);
        free(b);
        free(c_actual);
        free(mmultTarget);
        free(mmult_simdTarget);
        free(mmult_ompTarget);
        free(mmult_mpi_ompTarget);
    }
}
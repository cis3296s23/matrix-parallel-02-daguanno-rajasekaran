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

int generate_textified_matrices (){

    FILE * fp, * fp2, * fp3;
    srand(time(0));
    int matrixSize = (rand() % 1000) + 200;

    //open files to twrite to
    fp = fopen ("m1.txt", "w+");
    fp2 = fopen ("m2.txt", "w+");
    fp3 = fopen ("m3.txt", "w+");

    //write sizes of matrices to files
    fprintf(fp, "%d %d\n", matrixSize, matrixSize);
    fprintf(fp2, "%d %d\n", matrixSize, matrixSize);
    fprintf(fp3, "%d %d\n", matrixSize, matrixSize);

    //allocate memory for all matrices
    double *a = malloc(matrixSize * matrixSize * sizeof(double));
    double *b = malloc(matrixSize * matrixSize * sizeof(double));
    double *c_calc = malloc(matrixSize * matrixSize * sizeof(double));

    //generate matrices to be multiplied
    a = gen_matrix(matrixSize, matrixSize);
    b = gen_matrix(matrixSize, matrixSize);
        
    //calculate Target with mmult
    double *c_actual = mmult(c_calc, a, matrixSize, matrixSize, b, matrixSize, matrixSize);

    //write matrices from memory to files
    for(int i = 0; i < matrixSize; i++) {
        for(int j = 0; j < matrixSize; j++) {
            fprintf(fp, "%5lf ", a[matrixSize * i + j]);
        }
        //fprintf(fp, "\n");
    }

    for(int i = 0; i < matrixSize; i++) {
        for(int j = 0; j < matrixSize; j++) {
            fprintf(fp2, "%5lf ", b[matrixSize * i + j]);
        }
        //fprintf(fp2, "\n");
    }

    for(int i = 0; i < matrixSize; i++) {
        for(int j = 0; j < matrixSize; j++) {
            fprintf(fp3, "%5lf ", c_actual[matrixSize * i + j]);
        }
        //fprintf(fp3, "\n");
    }

    //close files
    fclose(fp);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

int main () {
    return generate_textified_matrices();
}
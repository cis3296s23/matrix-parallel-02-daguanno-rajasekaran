#include <stdio.h>
#include <stdlib.h>

#include "mat.h"
#include "mat.c"
#include "mmult.c"

#define MAT_SIZE 5

int test_unoptimized(double *a, int arows, int acols,
                     double *b, int brows, int bcols,
                     double *c_actual) {
    double *c_calc = malloc(MAT_SIZE * MAT_SIZE * sizeof(double));

    mmult(c_calc, a, MAT_SIZE, MAT_SIZE, b, MAT_SIZE, MAT_SIZE);

    int are_same = compare_matrices(c_actual, c_calc, MAT_SIZE, MAT_SIZE);

    free(c_calc);

    return are_same;
}

int main(void) {
    double *a = read_matrix_from_file("m1.txt");
    double *b = read_matrix_from_file("m2.txt");
    double *c_actual = read_matrix_from_file("m3.txt");
    double *c_calc = malloc(MAT_SIZE * MAT_SIZE * sizeof(double));

    if(!test_unoptimized(a, MAT_SIZE, MAT_SIZE, b, MAT_SIZE, MAT_SIZE, c_actual)) {
        exit(1);
    }

    puts("All tests pass.");

    free(a);
    free(b);
    free(c_actual);
}


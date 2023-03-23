#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define N 1000 // size of matrices

int main(int argc, char** argv) {
    int num_procs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // each process initializes its own chunk of matrices
    int chunk_size = N / num_procs;
    int A = (int)malloc(chunk_size * N * sizeof(int));
    int B = (int)malloc(chunk_size * N * sizeof(int));
    int C = (int)malloc(chunk_size * N * sizeof(int));

    // initialize matrices A and B
    for (int i = 0; i < chunk_size * N; i++) {
        A[i] = i;
        B[i] = i;
    }

    // multiply matrices using OpenMP parallelization
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[iN + k] B[kN + j];
            }
            C[iN + j] = sum;
        }
    }

    // collect all the chunks of C from each process
    MPI_Gather(C, chunk_size * N, MPI_INT, C, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // print out the resulting matrix C from process 0
    if (rank == 0) {
        printf("Matrix C:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i*N + j]);
            }
            printf("\n");
        }
    }

    // free memory
    free(A);
    free(B);
    free(C);

    MPI_Finalize();
    return 0;
}
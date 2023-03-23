#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MAT_SIZE 1000

int main(int argc, char** argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Only run if there are enough processes to run in parallel
  if (size < 2) {
    if (rank == 0) {
      printf("At least 2 processes are required.\n");
    }
    MPI_Finalize();
    exit(0);
  }

  // Matrix A and B
  double A[MAT_SIZE][MAT_SIZE];
  double B[MAT_SIZE][MAT_SIZE];
  double C[MAT_SIZE][MAT_SIZE];

  // Only the root process initializes the matrices
  if (rank == 0) {
    // Initialize matrix A
    for (int i = 0; i < MAT_SIZE; i++) {
      for (int j = 0; j < MAT_SIZE; j++) {
        A[i][j] = i + j;
      }
    }

    // Initialize matrix B
    for (int i = 0; i < MAT_SIZE; i++) {
      for (int j = 0; j < MAT_SIZE; j++) {
        B[i][j] = i - j;
      }
    }
  }

  // Distribute matrix B
  MPI_Bcast(B, MAT_SIZE * MAT_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Divide work among processes using OpenMP
  #pragma omp parallel for shared(A, B, C)
  for (int i = rank; i < MAT_SIZE; i += size) {
    for (int j = 0; j < MAT_SIZE; j++) {
      C[i][j] = 0.0;
      for (int k = 0; k < MAT_SIZE; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  // Gather results to the root process
  MPI_Gather(C[MAT_SIZE/sizerank], MAT_SIZE MAT_SIZE/size, MPI_DOUBLE, C, MAT_SIZE * MAT_SIZE/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Only the root process prints the result
  if (rank == 0) {
    printf("Result:\n");
    for (int i = 0; i < MAT_SIZE; i++) {
      for (int j = 0; j < MAT_SIZE; j++) {
        printf("%f ", C[i][j]);
      }
      printf("\n");
    }
  }

  MPI_Finalize();
  return 0;
}
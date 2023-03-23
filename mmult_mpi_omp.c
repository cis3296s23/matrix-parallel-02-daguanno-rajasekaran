/** 
 * Incomplete program to multiply a matrix times a matrix using both
 * MPI to distribute the computation among nodes and OMP
 * to distribute the computation among threads.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

#include "mat.h"

int main(int argc, char* argv[]) {

    int nrows, ncols;
    double *aa;	/* the A matrix */
    double *bb;	/* the B matrix */
    double *cc1;	/* A x B computed using the omp-mpi code you write */
    double *cc2;	/* A x B computed using the conventional algorithm */
    int myid, numprocs;
    double starttime, endtime;
    MPI_Status status;

    /* insert other global variables here */
    int stripesize, stripe, sender;
    int numsent = 0;
    double *a, *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (argc > 1) {

        //get and set number of rows and columns
        nrows = atoi(argv[1]);
        ncols = nrows;
        
        //set stripesize to number of workers
        stripesize = ncols/numprocs;

        
        //malloc for cc1(answer matrix)
        cc1 = malloc(sizeof(double) * nrows * ncols);

        //initialize for loop iterators
        int i, j, k;

        if (myid == 0) {// Controller Code goes here
            printf("matrix size: %d\n", ncols);
            printf("generate matrices\n");
            //generate matrices to multiply together
            aa = gen_matrix(nrows, ncols);
            bb = gen_matrix(ncols, nrows);

            printf("generated matrices\n");
        }

        if(myid != 0) { //worker code
            printf("hi i am worker %d\n", myid);

            int rows_per_process = nrows / numprocs;
            int remaining_rows = nrows % numprocs;

            //malloc for local_A and local_C
            int local_rows = rows_per_process + (myid < remaining_rows ? 1 : 0);
            double* local_A = (double*)malloc(local_rows * ncols * sizeof(double));
            double* local_C = (double*)malloc(local_rows * ncols * sizeof(double));

            printf("worker %d after malloc\n", myid);

            //set local_A to the workers assigned stripe
            for(i = 0; i < stripesize; i++) {
                local_A[i] += aa[myid * i];
            }

            // Perform local matrix multiplication
            for (int i = 0; i < stripesize; i++) {
                for (int j = 0; j < nrows; j++) {
                    local_C[i * nrows + j] = 0;
                    for (int k = 0; k < ncols; k++) {
                        local_C[i * nrows + j] += local_A[i * ncols + k] * bb[k * nrows + j];
                    }
                }
            }

            // set cc1 to local_C 
            for(i = 1; i < stripesize + 1; i++) {
                cc1[myid * i - 1] += local_C[i-1];
            }

            //broadcast cc1
            MPI_Bcast(cc1, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }

        if(myid == 0) { //controller code
            //start mpi timing
            starttime = MPI_Wtime();
            /* Insert your controller code here to store the product into cc1 */

            //broadcast bb (the matrix that each stripe is getting multiplied by)
            MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(aa, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //break array up into pieces
            int processrows = nrows / numprocs;
            int leftoverrows = nrows %numprocs;
        }

            // //malloc for local matrices
            // int localrows = processrows + (myid < leftoverrows ? 1 : 0);
            // double* localmatrix = (double*)malloc(sizeof(double) * ncols);
            // double* localleftovermatrix = (double*)malloc(sizeof(double) * ncols);

            // printf("after second malloc\n");

            // //Scatter chunks to processes
            // MPI_Scatterv(aa, stripes, startindex, MPI_DOUBLE, localmatrix, localrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // printf("after scatterv\n");
            
            // //broadcast bb (the matrix that each stripe is getting multiplied by)
            // MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // printf("after bcast\n");

            // //local matrix multiplication
            // for (int i = 0; i < localrows; i++) {
            //     for(int j = 0; j < ncols; j++) {
            //         localleftovermatrix[i * ncols +j] = 0;
            //         for(int k = 0; k < ncols; k++) {
            //             localleftovermatrix[i * ncols +j] += localmatrix[i *ncols + k] * bb[k * ncols + j];
            //         }
            //     }
            // }

            // printf("after matrix mult\n");
            
            // //gather
            // MPI_Barrier(MPI_COMM_WORLD);
            // MPI_Gatherv(localleftovermatrix, localrows * ncols, MPI_DOUBLE, cc1, stripes, startindex, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            // MPI_Barrier(MPI_COMM_WORLD);

            // printf("after Gather\n");
            
            MPI_Barrier(MPI_COMM_WORLD);
            print_matrix(cc1, nrows, ncols);
            MPI_Barrier(MPI_COMM_WORLD);
            
            //end MPI timing
            printf("mpi timing\n");
            endtime = MPI_Wtime();

            //print matrix size and time taken
            printf("matrix size %d", ncols);
            printf("%f\n",(endtime - starttime));

            //compare matrices with normal mmult
            cc2  = malloc(sizeof(double) * nrows * nrows);
            mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
            compare_matrices(cc2, cc1, nrows, nrows);

        } 
 
    //     if(myid != 0) {
    //         printf("hi i am worker %d\n", myid);

    //         int rows_per_process = nrows / numprocs;
    //         int remaining_rows = nrows % numprocs;

    //         int local_rows = rows_per_process + (myid < remaining_rows ? 1 : 0);
    //         double* local_A = (double*)malloc(local_rows * ncols * sizeof(double));
    //         double* local_C = (double*)malloc(local_rows * ncols * sizeof(double));

    //         printf("worker %d after malloc\n", myid);
    //     }

    //         printf("hi i am worker %d\n", myid);

    //         int rows_per_process = nrows / numprocs;
    //         int remaining_rows = nrows % numprocs;

    //         int local_rows = rows_per_process + (myid < remaining_rows ? 1 : 0);
    //         double* local_A = (double*)malloc(local_rows * ncols * sizeof(double));
    //         double* local_C = (double*)malloc(local_rows * ncols * sizeof(double));

    //         printf("worker %d after malloc\n", myid);

    //         //MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_A, local_rows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //         printf("worker %d after Scatter\n", myid);

    //         bb = (double*)realloc(bb, ncols * nrows * sizeof(double));

    //         printf("worker %d after realloc\n", myid);

    //         MPI_Bcast(bb, ncols * nrows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //         printf("worker %d after bcast\n", myid);

            

    //         printf("worker %d matrix mult\n", myid);
    //         print_matrix(local_C, nrows, ncols);

    //         free(local_A);
    //         free(local_C);
            
    //         printf("worker %d after free\n", myid);
            
    //     }
    // } else {
    //     fprintf(stderr, "Usage matrix_times_vector <size>\n");
    // }
    MPI_Finalize();
    return 0;
}


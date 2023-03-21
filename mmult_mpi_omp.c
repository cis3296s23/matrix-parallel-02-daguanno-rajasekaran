/** 
 * Incomplete program to multiply a matrix times a matrix using both
 * MPI to distribute the computation among nodes and OMP
 * to distribute the computation among threads.
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

#include "mat.h"

int main(int argc, char* argv[])
{
    int nrows, ncols;
    double *aa;	/* the A matrix */
    double *bb;	/* the B matrix */
    double *cc1;	/* A x B computed using the omp-mpi code you write */
    double *cc2;	/* A x B computed using the conventional algorithm */
    int myid, numprocs;
    double starttime, endtime;
    MPI_Status status;

    /* insert other global variables here */
    int stripesize;
    int numsent = 0;
    double *a, *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (argc > 1) {
        //get and set number of rows and columns
        nrows = atoi(argv[1]);
        ncols = nrows;
        printf("matrix size: %d\n", ncols);

        //set stripesize to number of slaves
        stripesize = ncols/3;

        //malloc for buffer, a, and b
        buffer = (double*)malloc(ncols * stripesize);
        a = (double*)malloc(sizeof(double) * nrows * ncols);
        aa = (double*)malloc(sizeof(double) * nrows * ncols);
        bb = (double*)malloc(sizeof(double) * nrows * ncols);

        if (myid == 0) {// Controller Code goes here
            printf("generate matrices\n");
            //generate matrices to multiply together
            aa = gen_matrix(nrows, ncols);
            bb = gen_matrix(ncols, nrows);

            //malloc for cc1(answer matrix)
            cc1 = malloc(sizeof(double) * nrows * nrows);

            //initialize for loop iterators
            int i, j, k, l;

            //start mpi timing
            starttime = MPI_Wtime();
            /* Insert your controller code here to store the product into cc1 */

            //broadcast bb (the matrix that each stripe is getting multiplied by)
            MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //for loop to send each stripe to a slave
            for(k = 0; k < 3; k++) {
            printf("earn your stripes\n");

                for (i = 0; i < stripesize; i++) {
                    for (j = 0; j < ncols; j++) {
                        buffer[i * ncols + j] = aa[i * ncols + j];
                    }
                }
                printf("buffer %d\n", k);
                print_matrix(buffer, ncols, stripesize);
                MPI_Send(buffer, ncols * stripesize, MPI_DOUBLE, k+1, k, MPI_COMM_WORLD);
                numsent++;
            }

            // while(numsent < ncols) {
                
            // }

            //receive stripes

            for (i = 0; i < 3; i++) {
                MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
                MPI_COMM_WORLD, &status);
            
                //get the stripe number
                int stripe = status.MPI_TAG;

                printf("buffer in loop\n");
                print_matrix(buffer, ncols, stripesize);

                //insert the stripe into the answer matrix cc1
                for (i = 0; i < stripesize * ncols; i++) {
                        cc1[stripe *ncols + i] = buffer[i];
                    }
                //MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
            }
                // MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
                //     MPI_COMM_WORLD, &status);
                
                // //get the stripe number
                // int stripe = status.MPI_TAG;

                // //insert the stripe into the answer matrix cc1
                // for(i = 0; i < nrows; i++) {
                //     for(j = 0; j < ncols; j++) {
                //         buffer[j] = cc1[i * ncols + j];
                //     }
                // }
            
            print_matrix(cc1, nrows, ncols);
            
            printf("mpi timing\n");
            //end MPI timing
            endtime = MPI_Wtime();
            printf("%f\n",(endtime - starttime));

            //compare matrices with normal mmult
            cc2  = malloc(sizeof(double) * nrows * nrows);
            mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
            compare_matrices(cc2, cc1, nrows, nrows);
        } else { // Worker code goes here
            
            //malloc buffer, a, and  for slaves
            //buffer = (double*)malloc(ncols * stripesize);
            a = (double*)malloc(sizeof(double) * ncols * stripesize);

            //broadcast matrix bb (the matrix that each stripe is getting multiplied by)
            MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //recieve buffer
            MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
                    MPI_COMM_WORLD, &status);
            int stripe = status.MPI_TAG;
            printf("stripe %d\n", stripe);

            //omp matrix mult of buffer(stripe) and bb to a
            int i, j, k = 0;
            #pragma omp parallel default(none) shared(a, bb, buffer, stripesize, ncols) private(i, k, j,stripe)
            #pragma omp for
            for (i = 0; i < stripesize; i++) {
                for (j = 0; j < ncols; j++) {
                    a[i*ncols + j] = 0;
                }
                for (k = 0; k < stripesize; k++) {
                    for (j = 0; j < ncols; j++) {
                        a[k * ncols + j] += buffer[k * stripesize + k] * bb[stripe * k * ncols + j];
                    }
                }
            }
            
            //send stripe back to controller
            MPI_Send(a, ncols * stripesize, MPI_DOUBLE, 0, stripe, MPI_COMM_WORLD);

            printf("print matrix from worker %d\n", stripe);
            print_matrix(a, nrows, stripesize);
            printf("worker %d done!\n", stripe);
        }
    } else {
        fprintf(stderr, "Usage matrix_times_vector <size>\n");
    }
    MPI_Finalize();
    return 0;
}
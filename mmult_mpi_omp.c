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

        //malloc for buffer, a, and b
        buffer = (double*)malloc(ncols * stripesize);
        a = (double*)malloc(sizeof(double) * ncols);
        aa = (double*)malloc(sizeof(double) * nrows * ncols);
        bb = (double*)malloc(sizeof(double) * nrows * ncols);

        if (myid == 0) {// Controller Code goes here
            printf("matrix size: %d\n", ncols);
            printf("generate matrices\n");
            //generate matrices to multiply together
            aa = gen_matrix(nrows, ncols);
            bb = gen_matrix(ncols, nrows);

            //malloc for cc1(answer matrix)
            cc1 = malloc(sizeof(double) * nrows * ncols);

            //initialize for loop iterators
            int i, j, k;

            //start mpi timing
            starttime = MPI_Wtime();
            /* Insert your controller code here to store the product into cc1 */


    
            
    

            //broadcast bb (the matrix that each stripe is getting multiplied by)
            MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //break array up into pieces
            int processrows = nrows / numprocs;
            int leftoverrows = nrows %numprocs;

            //something
            int stripes[numprocs];
            int startindex[numprocs];

            //break matrix into chunk
            for (int i = 0, offset = 0; i < numprocs; i++) {
                int assignedrows - processrows + (i < leftoverrows ? 1 : 0);
                stripes[i] = assignedrows * ncols;
                startindex[i] = offset;
                offset += stripes[i];
            }

            //malloc for local matrices
            int localrows = processrows + (myid < leftoverrows ? 1 : 0);
            double* localmatrix = (double*)malloc(sizeof(double) * ncols);
            double* localleftovermatrix = (double*)malloc(sizeof(double) * ncols);

            //Scatter chunks to processes
            MPI_Scatter(aa, stripes, startindex, MPI_DOUBLE, localmatrix, localrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //broadcast bb (the matrix that each stripe is getting multiplied by)
            MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //local matrix multiplication
            for (int i = 0; i < localrows; i++) {
                for(int j = 0; j < localrows; j++) {
                    localleftovermatrix[i * ncols +j] = 0;
                    for(int k = 0; k < ncols; k++) {
                        localmatrix[i * ncols +j] += localmatrix[i *ncols + k] * bb[k * ncols + j]
                    }
                }
            }

            //gather
            MPI_Gather(localmatrix, localrows * ncols, MPI_DOUBLE, cc1, stripes, startindex, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            //gg




            /*

            //for loop to send each stripe to a worker
            printf("earn your stripes\n");

            //send out first lines to workers
            for (i = 0; i < min(nrows, numprocs-1); i++) {
                for (j = 0; j < ncols; j++) {
                    buffer[j] = aa[i * ncols + j];
                }
            printf("buffer %d\n", k);
            print_matrix(buffer, ncols, stripesize);
            MPI_Send(buffer, ncols * stripesize, MPI_DOUBLE, i+1, i+1, MPI_COMM_WORLD);
            numsent++;
            }

            //receive lines from workers, add them to cc1, and then send them a new line unntil there are no more lines
            for (i = 0; i < nrows; i++) {
                //receive line
                MPI_Recv(buffer, stripesize * ncols, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sender = status.MPI_SOURCE;
                stripe = status.MPI_TAG;

                //add line to cc1
                for (i = 0; i < stripesize * ncols; i++) {
                    cc1[stripe *ncols + i] = buffer[i];
                }

                printf("numsent: %d\n", numsent);
                //send the remaining lines to workers
                if (numsent < nrows) {
                    for (j = 0; j < ncols; j++) {
                        buffer[j] = aa[numsent*ncols + j];
                    }  
                    MPI_Send(buffer, ncols, MPI_DOUBLE, sender, numsent+1, MPI_COMM_WORLD);
                    numsent++;
                    printf("numsent: %d\n", numsent);
                } else {
                    printf("ending process: %d\n", sender);
                    MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
                }
            }

            */

            //receive stripes

            // for (i = 0; i < 3; i++) {
            //     MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
            //     MPI_COMM_WORLD, &status);
            
            //     //get the stripe number
            //     int stripe = status.MPI_TAG;

            //     printf("buffer in loop\n");
            //     print_matrix(buffer, ncols, stripesize);

            //     //insert the stripe into the answer matrix cc1
            //     for (i = 0; i < stripesize * ncols; i++) {
            //             cc1[stripe *ncols + i] = buffer[i];
            //         }
            //     //MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
            // }
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
            printf("matrix size %d", ncols);
            printf("%f\n",(endtime - starttime));

            //compare matrices with normal mmult
            cc2  = malloc(sizeof(double) * nrows * nrows);
            mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
            compare_matrices(cc2, cc1, nrows, nrows);



        } else { // Worker code goes here

            int rows_per_process = nrows / numprocs;
            int remaining_rows = nrows % numprocs;

            int local_rows = rows_per_process + (myid < remaining_rows ? 1 : 0);
            double* local_A = (double*)malloc(local_rows * ncols * sizeof(double));
            double* local_C = (double*)malloc(local_rows * x * sizeof(double));

            MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_A, local_rows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            bb = (double*)realloc(bb, ncols * x * sizeof(double));
            MPI_Bcast(bb, ncols * x, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // Perform local matrix multiplication
            for (int i = 0; i < local_rows; i++) {
                for (int j = 0; j < x; j++) {
                    local_C[i * x + j] = 0;
                    for (int k = 0; k < ncols; k++) {
                        local_C[i * x + j] += local_A[i * ncols + k] * bb[k * x + j];
                    }
                }
            }

            // Gather results from all processes
            MPI_Gatherv(local_C, local_rows * x, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            free(local_A);
            free(local_C);
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            //broadcast matrix bb (the matrix that each stripe is getting multiplied by)
            // MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // if (myid <= nrows) {

            // int rank = myid;
                
            // #pragma omp parallel private(i, j, k, stripesize, start, end)
            // {
            //     start = rank * stripesize;
            //     end = (rank + 1) * stripesize;
            //     if (rank == numprocs-1) {
            //         end = ncols;
            //     }
            //     #pragma omp for schedule(static)
            //     for (i = start; i < end; i++) {
            //         for (j = 0; j < ncols; j++) {
            //             for (k = 0; k < ncols; k++) {
            //                 localmatrix[i][j] += a[i][k] * bb[k][j];
            //             }
            //         }
            //     }
            // }




                // while(1) {
                //     printf("worker %d start!\n", stripe);
                //     //recieve buffer, break if the tag is 0
                //     MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        
                //     //printf("stripe %d\n", stripe);
                //     if (status.MPI_TAG == 0){
                //         break;
                //     }

                //     int stripe = status.MPI_TAG;
                //     //omp matrix mult of buffer(stripe) and bb to a
                //     int i, j, k = 0;
                //     #pragma omp parallel default(none) shared(a, bb, buffer, stripesize, ncols) private(i, k, j,stripe)
                //     #pragma omp for
                //     for (i = 0; i < stripesize; i++) {
                //         for (j = 0; j < ncols; j++) {
                //             a[i*ncols + j] = 0;
                //         }
                //         for (k = 0; k < stripesize; k++) {
                //             for (j = 0; j < ncols; j++) {
                //                 a[k * ncols + j] += buffer[k * stripesize + k] * bb[stripe * k * ncols + j];
                //             }
                //         }
                //     }

                //     //send stripe back to controller
                //     MPI_Send(a, ncols * stripesize, MPI_DOUBLE, 0, stripe, MPI_COMM_WORLD);

                //     printf("print line from worker %d\n", stripe);
                //     print_matrix(a, nrows, stripesize);
                //     printf("worker %d done!\n", stripe);
                // }
            }
        //     //recieve buffer
        //     MPI_Recv(buffer, ncols * stripesize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
        //             MPI_COMM_WORLD, &status);
        //     int stripe = status.MPI_TAG;
        //     printf("stripe %d\n", stripe);

        //     //omp matrix mult of buffer(stripe) and bb to a
        //     int i, j, k = 0;
        //     #pragma omp parallel default(none) shared(a, bb, buffer, stripesize, ncols) private(i, k, j,stripe)
        //     #pragma omp for
        //     for (i = 0; i < stripesize; i++) {
        //         for (j = 0; j < ncols; j++) {
        //             a[i*ncols + j] = 0;
        //         }
        //         for (k = 0; k < stripesize; k++) {
        //             for (j = 0; j < ncols; j++) {
        //                 a[k * ncols + j] += buffer[k * stripesize + k] * bb[stripe * k * ncols + j];
        //             }
        //         }
        //     }
            
        //     //send stripe back to controller
        //     MPI_Send(a, ncols * stripesize, MPI_DOUBLE, 0, stripe, MPI_COMM_WORLD);

        //     printf("print matrix from worker %d\n", stripe);
        //     print_matrix(a, nrows, stripesize);
        //     printf("worker %d done!\n", stripe);
        }
    } else {
        fprintf(stderr, "Usage matrix_times_vector <size>\n");
    }
    MPI_Finalize();
    return 0;
}
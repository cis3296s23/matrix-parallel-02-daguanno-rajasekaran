#!/bin/bash

# Compile gen_gnu.c and gen_gnu_O3.c
echo "compiling gen_gnu"
gcc -o gen_gnu gen_gnu.c -D_POSIX_C_SOURCE=199309L -lrt -std=c11 -fopenmp
echo "compiling gen_gnu_O3"
gcc -O3 gen_gnu_O3.c -o gen_gnu_O3

# Compile mmult_mpi.c with MPI
echo "compiling mpi"
mpicc -o mmult_mpi -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

# Compile mmult_mpi_omp.c with MPI and OpenMP
echo "compiling mpi omp"
mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

# Run the programs
echo "running gen_gnu"
./gen_gnu
echo "running gen_gnu_O3"
./gen_gnu_O3

# Define the start and end values for the loop
start=60
end=3000
step=60

# Loop over the argument values for mmult_mpi
echo "running mpi"
for ((i=start; i<=end; i+=step))
do
    echo "Calculating MPI size: $i"
    mpiexec -f ~/hosts -n 4 ./mmult_mpi $i
done

# Loop over the argument values for mmult_mpi_omp
echo "running mpi omp"
for ((i=start; i<=end; i+=step))
do
    echo "Calculating MPI_OMP size: $i"
    mpiexec -f ~/hosts -n 4 ./mmult_mpi_omp $i
done
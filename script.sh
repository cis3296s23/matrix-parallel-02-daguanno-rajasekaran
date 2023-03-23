#!/bin/bash

# Compile gen_gnu.c and gen_gnu_O3.c
echo "compiling gen_gnu's"
gcc -o gen_gnu gen_gnu.c
gcc -o gen_gnu_O3 -O3 gen_gnu_O3.c

# Compile mmult_mpi.c with MPI
echo "compiling mpi"
mpicc -o mmult_mpi -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

# Compile mmult_mpi_omp.c with MPI and OpenMP
echo "compiling mpi omp"
mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

# Run the programs
echo "running gen_gnu"
./gen_gnu
echo "gen_gnu_O3"
./gen_gnu_O3

# Define the start and end values for the loop
start=60
end=6000
step=60

# Loop over the argument values for mmult_mpi
for ((i=start; i<=end; i+=step))
do
    echo "Running mmult_mpi with $i argument(s)..."
    mpiexec -f ~/hosts -n 4 ./mmult_mpi_omp $i
done

# Loop over the argument values for mmult_mpi_omp
for ((i=start; i<=end; i+=step))
do
    echo "Running mmult_mpi_omp with $i argument(s)..."
    mpiexec -f ~/hosts -n 4 ./mmult_mpi_omp $i
done
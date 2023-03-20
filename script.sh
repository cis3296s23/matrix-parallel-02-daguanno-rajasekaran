!# /bin/bash
i = 50
mpicc mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

mpiexec -f ~/hosts -n 4 ./mmult_mpi_omp 4000
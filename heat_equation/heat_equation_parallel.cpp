#include <mpi.h>

#include "heat_equation_parallel.hpp"


int main(int argc, char **argv)
{
    // default values
    int cols_tot = 16;
    int rows_tot = 16;

    // init MPI
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    // get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    // get current rank of current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // determine the number of rows of the process
    int rows_proc = 2 + (rows_tot / nprocs) + ((rows_tot % nprocs > rank) ? 1 : 0);

    // initialize the two matrices
    Mat U(rows_proc, cols_tot);
    Mat U_new(rows_proc, cols_tot);

    // initilize the internal rows, overlapping regions and the boundaries of the matrices
    for (int i = 0; i < rows_proc; i++) {
        for (int j = 0; j < cols_tot; j++) {
            U(i, j) = 0.0;
        }
    }

    // initialize the last internal row of the last process to 100
    if (rank == nprocs - 1) {
        for (int j = 0; j < cols_tot; j++) {
            U(rows_proc - 2, j) = 100.0;
        }
    }

    // end MPI
    MPI_Finalize();

    return 0;
}

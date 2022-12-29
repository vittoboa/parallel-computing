#include <mpi.h>

int main(int argc, char **argv)
{
    // init MPI
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    // get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    // get current rank of current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // end MPI
    MPI_Finalize();

    return 0;
}

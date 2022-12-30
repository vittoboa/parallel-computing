#include <mpi.h>
#include <cmath>

#include "heat_equation_parallel.hpp"


int main(int argc, char **argv)
{
    // default values
    int cols_tot = 16;
    int rows_tot = 16;
    double epsilon = 1.0e-3;
    int max_iterations = 15000;

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

    double diffnorm;
    int iteration_count = 0;
    do {
        iteration_count++;
        diffnorm = 0.0;

        // define variables for sending and receving requests
        MPI_Status stat;
        MPI_Request reqsend[2], reqrecv[2];

        // send and receive rows to update overlapping regions
        if (rank < nprocs - 1) {
            MPI_Irecv(&U(rows_proc - 1, 1), cols_tot - 2, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &reqrecv[0]);  // receive from below (rank + 1)
            MPI_Isend(&U(rows_proc - 2, 1), cols_tot - 2, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &reqsend[0]);  // send below (rank + 1)
        }
        if (rank > 0) {
            MPI_Irecv(&U(0, 1), cols_tot - 2, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &reqrecv[1]);  // receive from above (rank - 1)
            MPI_Isend(&U(1, 1), cols_tot - 2, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &reqsend[1]);  // send above (rank - 1)
        }

        // compute new values, but not on the boundaries and not on the first and last row
        int start = (rank > 0) ? 1 : 2;
        int end = (rank < nprocs - 1) ? rows_proc - 2 : rows_proc - 3;
        for (int i = start + 1; i <= end - 1; ++i) {
            for (int j = 1; j < cols_tot - 1; ++j) {
                // compute the new updated values
                U_new(i, j) = (U(i, j + 1) + U(i, j - 1) + U(i + 1, j) + U(i - 1, j)) * 0.25;
                // keep track of how much the values have changed since the previous iteration
                diffnorm += (U_new(i, j) - U(i, j)) * (U_new(i, j) - U(i, j));
            }
        }

        // wait until the data of the overlapping regions has been received
        if (rank < nprocs - 1) {
            MPI_Wait(&reqrecv[0], &stat);
        }
        if (rank > 0) {
            MPI_Wait(&reqrecv[1], &stat);
        }

        // update the first and last rows, which depend on overlapping regions
        if (start <= end) {
            for (int j = 1; j < cols_tot - 1; ++j) {
                // compute the new updated values
                U_new(start, j) = (U(start, j + 1) + U(start, j - 1) + U(start + 1, j) + U(start - 1, j)) * 0.25;
                // keep track of how much the values have changed since the previous iteration
                diffnorm += (U_new(start, j) - U(start, j)) * (U_new(start, j) - U(start, j));
            }
        }
        if (end > start) {
            for (int j = 1; j < cols_tot - 1; ++j) {
                // compute the new updated values
                U_new(end, j) = (U(end, j + 1) + U(end, j - 1) + U(end + 1, j) + U(end - 1, j)) * 0.25;
                // keep track of how much the values have changed since the previous iteration
                diffnorm += (U_new(end, j) - U(end, j)) * (U_new(end, j) - U(end, j));
            }
        }

        // wait until the first and last internal rows have been sent before updating the matrix U
        if (rank < nprocs - 1) {
            MPI_Wait(&reqsend[0], &stat);
        }
        if (rank > 0) {
            MPI_Wait(&reqsend[1], &stat);
        }

        // update the internal points of the matrix U
        start = (rank > 0) ? 1 : 2;
        end = (rank < nprocs - 1) ? rows_proc - 2 : rows_proc - 3;
        for (int i = start; i <= end; ++i) {
            for (int j = 1; j < cols_tot - 1; ++j) {
                U(i, j) = U_new(i, j);
            }
        }

        // sum the diffnorm of all processes which is used to determine if the iteration should stop
        MPI_Allreduce(MPI_IN_PLACE, &diffnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        diffnorm = sqrt(diffnorm);

    } while (epsilon <= diffnorm && iteration_count < max_iterations);

    // end MPI
    MPI_Finalize();

    return 0;
}

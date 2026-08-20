#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int x = 3; // Integer constant base
    int result = 1;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Calculate x^rank using a simple loop
    // (For rank 0, result remains 1 since x^0 = 1)
    for (int i = 0; i < rank; i++) {
        result *= x;
    }

    // Print the result from each process
    printf("Process %d of %d: %d^%d = %d\n", rank, size, x, rank, result);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
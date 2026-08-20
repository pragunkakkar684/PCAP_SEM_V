#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if the rank is even or odd
    if (rank % 2 == 0) {
        printf("Process %d: Hello\n", rank);
    } else {
        printf("Process %d: World\n", rank);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
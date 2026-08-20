#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    double num1 = 20.0, num2 = 4.0; // Input operands for the calculator
    double result = 0.0;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get current process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ensure we have at least 4 processes for the 4 operations
    if (size < 4) {
        if (rank == 0) {
            printf("Error: Please run this program with at least 4 processes (-np 4).\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Each process performs a different operation based on its rank
    switch (rank) {
        case 0:
            result = num1 + num2;
            printf("Process 0 (Addition): %.1f + %.1f = %.1f\n", num1, num2, result);
            break;
        case 1:
            result = num1 - num2;
            printf("Process 1 (Subtraction): %.1f - %.1f = %.1f\n", num1, num2, result);
            break;
        case 2:
            result = num1 * num2;
            printf("Process 2 (Multiplication): %.1f * %.1f = %.1f\n", num1, num2, result);
            break;
        case 3:
            if (num2 != 0) {
                result = num1 / num2;
                printf("Process 3 (Division): %.1f / %.1f = %.1f\n", num1, num2, result);
            } else {
                printf("Process 3 (Division): Error! Division by zero.\n");
            }
            break;
        default:
            // Any extra processes beyond 4 won't do calculator tasks here
            if (rank < size) {
                printf("Process %d: Idle (no assigned operation)\n", rank);
            }
            break;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
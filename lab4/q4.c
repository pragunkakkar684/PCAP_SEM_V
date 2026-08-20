#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    // The string to be modified (must be large enough or matched to process count)
    char str[] = "HeLLO"; 

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get current process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ensure the number of processes matches the string length for this example
    int len = strlen(str);
    if (size < len) {
        if (rank == 0) {
            printf("Error: Please run with at least %d processes (-np %d).\n", len, len);
        }
        MPI_Finalize();
        return 0;
    }

    // Each process toggles the character at its specific rank index
    if (rank < len) {
        char original = str[rank];
        if (islower(original)) {
            str[rank] = toupper(original);
        } else if (isupper(original)) {
            str[rank] = tolower(original);
        }

        printf("Process %d toggled '%c' to '%c'\n", rank, original, str[rank]);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
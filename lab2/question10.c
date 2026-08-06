#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int M, N;

    // Read dimensions of the matrix
    printf("Pragun Kakkar - 240968332\n");
    printf("Enter number of rows (M): ");
    if (scanf("%d", &M) != 1 || M <= 0) return 1;
    printf("Enter number of columns (N): ");
    if (scanf("%d", &N) != 1 || N <= 0) return 1;

    // Allocate memory for matrices A and B
    int **A = (int **)malloc(M * sizeof(int *));
    int **B = (int **)malloc(M * sizeof(int *));
    for (int i = 0; i < M; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
    }

    // Input matrix elements
    printf("Enter the elements of the %dx%d matrix:\n", M, N);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    // Process the matrix in parallel using OpenMP
    // We only process non-border elements: rows 1 to M-2 and cols 1 to N-2
    #pragma omp parallel for collapse(2) default(none) shared(A, B, M, N)
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == M - 1 || j == 0 || j == N - 1) {
                // Border elements remain the same
                B[i][j] = A[i][j];
            } else {
                // Non-border elements replaced with 1's complement (bitwise NOT)
                B[i][j] = ~A[i][j];
            }
        }
    }

    // Display the output matrix B
    printf("\nOutput Matrix B (Border same, Non-border 1's complement):\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d \t", B[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < M; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    return 0;
}
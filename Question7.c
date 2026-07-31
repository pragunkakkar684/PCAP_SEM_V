#include <stdio.h>

int main() {
    int A[5][5], B[5][5];
    int row_max[5], row_min[5];

    printf("Enter elements of the 5x5 matrix A:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    for (int i = 0; i < 5; i++) {
        int max_val = A[i][0];
        int min_val = A[i][0];

        for (int j = 1; j < 5; j++) {
            if (A[i][j] > max_val) {
                max_val = A[i][j];
            }
            if (A[i][j] < min_val) {
                min_val = A[i][j];
            }
        }
        row_max[i] = max_val;
        row_min[i] = min_val;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == j) {
                B[i][j] = 0;
            } 
            else if (i > j) {
                B[i][j] = row_max[i];
            } 
            else {
                B[i][j] = row_min[i];
            }
        }
    }

    printf("\nResultant Matrix B:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    return 0;
}

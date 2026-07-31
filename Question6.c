#include <stdio.h>

int main() {
    int m, n;
    float A[10][10], x[10], y[10];

    printf("Enter number of rows (m) for matrix: ");
    scanf("%d", &m);
    printf("Enter number of columns (n) for matrix (size of vector): ");
    scanf("%d", &n);

    printf("\nEnter elements of the %d x %d matrix A:\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("A[%d][%d]: ", i, j);
            scanf("%f", &A[i][j]);
        }
    }

    printf("\nEnter %d elements for the vector x:\n", n);
    for (int i = 0; i < n; i++) {
        printf("x[%d]: ", i);
        scanf("%f", &x[i]);
    }

    for (int i = 0; i < m; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    printf("\n Resulting Vector y (A * x)\n");
    for (int i = 0; i < m; i++) {
        printf("y[%d] = %.2f\n", i, y[i]);
    }

    return 0;
}

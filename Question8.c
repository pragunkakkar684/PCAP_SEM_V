#include <stdio.h>

int main() {
    int m, n;
    
    printf("Enter number of rows (M): ");
    scanf("%d", &m);
    printf("Enter number of columns (N): ");
    scanf("%d", &n);

    int A[100][100], B[100][100], D[100][100];

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = A[i][j];
            D[i][j] = A[i][j];
        }
    }

    for (int i = 1; i < m - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
 
        }
    }
    
    printf("\nResultant Matrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", B[i][j]);
        }
        printf("\n");
    }

    printf("\nResultant Matrix D:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", D[i][j]);
        }
        printf("\n");
    }

    return 0;
}

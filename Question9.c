#include <stdio.h>

int main() {
    int m, n;

    printf("Enter number of rows (M): ");
    scanf("%d", &m);
    printf("Enter number of columns (N): ");
    scanf("%d", &n);

    char A[100][100];
    int B[100][100];
    char STR[1000];
    int k = 0; 

    printf("Enter elements of character matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf(" %c", &A[i][j]); 
        }
    }

    printf("Enter elements of integer matrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int r = 0; r < B[i][j]; r++) {
                STR[k++] = A[i][j];
            }
        }
    }
    
    STR[k] = '\0';

    printf("\noutput string STR: %s\n", STR);

    return 0;
}

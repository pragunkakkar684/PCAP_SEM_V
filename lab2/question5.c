#include <stdio.h>
#include <omp.h>

int main() {
    int start, end, i, j, prime;

    printf("Pragun Kakkar - 240968332\n");

    printf("Enter the starting number: ");
    scanf("%d", &start);

    printf("Enter the ending number: ");
    scanf("%d", &end);

    printf("\nPrime numbers between %d and %d are:\n", start, end);

    // Parallelize the loop
    #pragma omp parallel for private(i, j, prime) schedule(dynamic)
    for (i = start; i <= end; i++) {

        if (i < 2)
            continue;

        prime = 1;

        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }

        if (prime) {
            #pragma omp critical
            printf("%d (Thread %d)\n", i, omp_get_thread_num());
        }
    }

    return 0;
}
#include <stdio.h>
#include <omp.h>

int main() {
    int n, i;
    int evenSum = 0, oddSum = 0;
    printf("Pragun Kakkar - 240968332\n");

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d elements:\n", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Create exactly 2 threads
    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();

        if(tid == 0) {
            // Thread 0: Sum of even numbers
            for(i = 0; i < n; i++) {
                if(arr[i] % 2 == 0)
                    evenSum += arr[i];
            }
            printf("Thread %d calculated Even Sum = %d\n", tid, evenSum);
        }
        else {
            // Thread 1: Sum of odd numbers
            for(i = 0; i < n; i++) {
                if(arr[i] % 2 != 0)
                    oddSum += arr[i];
            }
            printf("Thread %d calculated Odd Sum = %d\n", tid, oddSum);
        }
    }

    printf("\nFinal Even Sum = %d\n", evenSum);
    printf("Final Odd Sum = %d\n", oddSum);

    return 0;
}
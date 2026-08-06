#include <stdio.h>
#include <omp.h>

int main() {
    float a, b;

    printf("Pragun Kakkar - 240968332\n");
    printf("Enter two numbers: ");
    scanf("%f %f", &a, &b);

    // Create 4 threads
    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        switch(tid) {
            case 0:
                printf("Thread %d: Addition = %.2f\n", tid, a + b);
                break;

            case 1:
                printf("Thread %d: Subtraction = %.2f\n", tid, a - b);
                break;

            case 2:
                printf("Thread %d: Multiplication = %.2f\n", tid, a * b);
                break;

            case 3:
                if (b != 0)
                    printf("Thread %d: Division = %.2f\n", tid, a / b);
                else
                    printf("Thread %d: Division not possible (Division by zero)\n", tid);
                break;
        }
    }

    return 0;
}
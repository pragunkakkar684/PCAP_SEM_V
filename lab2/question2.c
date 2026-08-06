#include <stdio.h>
#include <omp.h>

int main() {
    int i = 10;   // Integer value
    printf("Pragun Kakkar - 240968332\n");

    #pragma omp parallel
    {
        int x = omp_get_thread_num();   // Thread ID

        printf("Thread %d : (i, x) = (%d, %d)\n", x, i, x);
    }

    return 0;
}
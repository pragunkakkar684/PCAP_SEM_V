#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int a[10];
    printf("Pragun Kakkar - 240968332");

    // SPMD Pattern
    #pragma omp parallel
    {
        #pragma omp for
        for(i = 0; i < 10; i++) {
            a[i] = i * i;
            printf("Thread %d processed a[%d] = %d\n",
                   omp_get_thread_num(), i, a[i]);
        }
    }

    printf("\nFinal Array:\n");
    for(i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");
    return 0;
}
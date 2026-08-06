#include <stdio.h>
#include <omp.h>

int main() {
    printf("Pragun Kakkar - 240968332");
    printf("Serial region: Before parallel block\n");

    // Fork
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from Thread %d\n", tid);
    }
    // Join

    printf("Serial region: After parallel block\n");

    return 0;
}
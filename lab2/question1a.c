#include <stdio.h>
#include <omp.h>

int main() {

    printf("Pragun Kakkar - 240968332\n");
    printf("Parallel Region 1 (Using omp_set_num_threads)\n");

    // Method 1: Set number of threads using function
    omp_set_num_threads(2);

    #pragma omp parallel
    {
        printf("Thread %d of %d\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    printf("\nParallel Region 2 (Using num_threads clause)\n");

    // Method 2: Using num_threads clause
    #pragma omp parallel num_threads(4)
    {
        printf("Thread %d of %d\n",
               omp_get_thread_num(),
               omp_get_num_threads());
    }

    return 0;
}
#include <stdio.h>
#include <omp.h>

int fibonacci(int n)
{
    if(n <= 1)
        return n;

    int a = 0, b = 1, c;

    for(int i = 2; i <= n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }

    return b;
}

int main()
{
    int A[] = {10, 13, 5, 6};
    int result[4];
    int threads[] = {2,4,6,8};
    printf("Pragun Kakkar - 240968332\n");

    double serial_time, parallel_time;

    // ---------------- Serial Execution ----------------

    double start = omp_get_wtime();

    for(int i=0;i<4;i++)
        result[i] = fibonacci(A[i]);

    double end = omp_get_wtime();

    serial_time = end - start;

    printf("\nSerial Fibonacci Results\n");
    for(int i=0;i<4;i++)
        printf("Fib(%d) = %d\n",A[i],result[i]);

    printf("\nSerial Time = %lf seconds\n",serial_time);

    // ---------------- Parallel Execution ----------------

    printf("\n---------------------------------------------\n");
    printf("Threads\tTime\t\tSpeedup\tEfficiency\n");
    printf("---------------------------------------------\n");

    for(int k=0;k<4;k++)
    {
        omp_set_num_threads(threads[k]);

        start = omp_get_wtime();

        #pragma omp parallel for
        for(int i=0;i<4;i++)
        {
            result[i] = fibonacci(A[i]);

            printf("Thread %d computed Fib(%d) = %d\n",
                    omp_get_thread_num(),
                    A[i],
                    result[i]);
        }

        end = omp_get_wtime();

        parallel_time = end-start;

        double speedup = serial_time/parallel_time;
        double efficiency = speedup/threads[k];

        printf("%d\t%lf\t%lf\t%lf\n",
               threads[k],
               parallel_time,
               speedup,
               efficiency);

        printf("---------------------------------------------\n");
    }

    return 0;
}
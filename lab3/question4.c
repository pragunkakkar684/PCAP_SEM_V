#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_SIZES 5
#define NUM_THREAD_COUNTS 4

/* ---------------------------------------------------------
   Serial Sum
   --------------------------------------------------------- */
long long sum_serial(long long *a, long long n)
{
    long long sum = 0;

    for (long long i = 0; i < n; i++)
        sum += a[i];

    return sum;
}

/* ---------------------------------------------------------
   Sum using CRITICAL
   --------------------------------------------------------- */
long long sum_critical(long long *a, long long n, int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel
    {
        long long local_sum = 0;

        #pragma omp for
        for (long long i = 0; i < n; i++)
        {
            local_sum += a[i];
        }

        #pragma omp critical
        {
            sum += local_sum;
        }
    }

    return sum;
}

/* ---------------------------------------------------------
   Sum using ATOMIC
   --------------------------------------------------------- */
long long sum_atomic(long long *a, long long n, int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for
    for (long long i = 0; i < n; i++)
    {
        #pragma omp atomic
        sum += a[i];
    }

    return sum;
}

/* ---------------------------------------------------------
   Sum using REDUCTION
   --------------------------------------------------------- */
long long sum_reduction(long long *a, long long n, int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < n; i++)
    {
        sum += a[i];
    }

    return sum;
}

/* ---------------------------------------------------------
   Sum using MASTER
   --------------------------------------------------------- */
long long sum_master(long long *a, long long n, int threads)
{
    long long sum = 0;

    long long *local_sum =
        (long long *)calloc(threads, sizeof(long long));

    if (local_sum == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    omp_set_num_threads(threads);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        /*
         * Each thread calculates its own partial sum.
         */
        #pragma omp for
        for (long long i = 0; i < n; i++)
        {
            local_sum[tid] += a[i];
        }

        /*
         * Only the master thread combines the
         * partial sums.
         */
        #pragma omp master
        {
            for (int i = 0; i < threads; i++)
            {
                sum += local_sum[i];
            }
        }
    }

    free(local_sum);

    return sum;
}

/* ---------------------------------------------------------
   Sum using LOCKS
   --------------------------------------------------------- */
long long sum_locks(long long *a, long long n, int threads)
{
    long long sum = 0;

    omp_lock_t lock;

    omp_init_lock(&lock);

    omp_set_num_threads(threads);

    #pragma omp parallel for
    for (long long i = 0; i < n; i++)
    {
        omp_set_lock(&lock);

        sum += a[i];

        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    return sum;
}

/* ---------------------------------------------------------
   Save result to CSV
   --------------------------------------------------------- */
void save_result(
    FILE *fp,
    const char *method,
    long long n,
    int threads,
    double serial_time,
    double parallel_time)
{
    double speedup =
        serial_time / parallel_time;

    double efficiency =
        (speedup / threads) * 100.0;

    fprintf(
        fp,
        "%s,%lld,%d,%.8f,%.8f,%.6f,%.4f\n",
        method,
        n,
        threads,
        serial_time,
        parallel_time,
        speedup,
        efficiency
    );
}

/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */
int main()
{
    /*
     * Input sizes
     */
    long long sizes[NUM_SIZES] = {
        1000000,
        5000000,
        10000000,
        20000000,
        50000000
    };

    /*
     * ONLY 4 THREAD COUNTS
     */
    int thread_counts[NUM_THREAD_COUNTS] = {
        1, 2, 3, 4
    };

    FILE *fp;

    /*
     * REQUIRED FIRST OUTPUT LINE
     */
    printf("Pragun Kakkar - 240968332\n");

    printf("\nOpenMP Sum of N Numbers\n");
    printf("=======================\n");

    printf("Constructs: Critical, Atomic, Reduction, Master, Locks\n");
    printf("Threads tested: 1, 2, 3, 4\n");

    /*
     * Open CSV file
     */
    fp = fopen("sum_openmp_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating CSV file.\n");
        return 1;
    }

    fprintf(
        fp,
        "Method,InputSize,Threads,SerialTime,ParallelTime,Speedup,Efficiency\n"
    );

    /*
     * =====================================================
     * LOOP THROUGH INPUT SIZES
     * =====================================================
     */
    for (int s = 0; s < NUM_SIZES; s++)
    {
        long long n = sizes[s];

        /*
         * Allocate array
         */
        long long *a =
            (long long *)malloc(n * sizeof(long long));

        if (a == NULL)
        {
            printf("Memory allocation failed for %lld.\n", n);
            fclose(fp);
            return 1;
        }

        /*
         * Fill array with 1.
         *
         * Therefore:
         *
         * Sum = N
         */
        for (long long i = 0; i < n; i++)
        {
            a[i] = 1;
        }

        /*
         * =================================================
         * SERIAL EXECUTION
         * =================================================
         */
        double start = omp_get_wtime();

        long long expected =
            sum_serial(a, n);

        double end = omp_get_wtime();

        double serial_time = end - start;

        /*
         * Print only one line for each input size.
         */
        printf("\nInput Size = %lld | Expected Sum = %lld\n",
               n,
               expected);

        /*
         * =================================================
         * TEST ALL THREAD COUNTS
         * =================================================
         */
        for (int t = 0; t < NUM_THREAD_COUNTS; t++)
        {
            int threads = thread_counts[t];

            long long result;

            double parallel_time;
            double speedup;
            double efficiency;

            /*
             * ---------------------------------------------
             * CRITICAL
             * ---------------------------------------------
             */
            start = omp_get_wtime();

            result =
                sum_critical(a, n, threads);

            end = omp_get_wtime();

            parallel_time = end - start;

            if (result != expected)
            {
                printf("ERROR: Critical result incorrect!\n");
                free(a);
                fclose(fp);
                return 1;
            }

            speedup = serial_time / parallel_time;
            efficiency =
                (speedup / threads) * 100.0;

            save_result(
                fp,
                "Critical",
                n,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * ---------------------------------------------
             * ATOMIC
             * ---------------------------------------------
             */
            start = omp_get_wtime();

            result =
                sum_atomic(a, n, threads);

            end = omp_get_wtime();

            parallel_time = end - start;

            if (result != expected)
            {
                printf("ERROR: Atomic result incorrect!\n");
                free(a);
                fclose(fp);
                return 1;
            }

            speedup = serial_time / parallel_time;
            efficiency =
                (speedup / threads) * 100.0;

            save_result(
                fp,
                "Atomic",
                n,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * ---------------------------------------------
             * REDUCTION
             * ---------------------------------------------
             */
            start = omp_get_wtime();

            result =
                sum_reduction(a, n, threads);

            end = omp_get_wtime();

            parallel_time = end - start;

            if (result != expected)
            {
                printf("ERROR: Reduction result incorrect!\n");
                free(a);
                fclose(fp);
                return 1;
            }

            speedup = serial_time / parallel_time;
            efficiency =
                (speedup / threads) * 100.0;

            save_result(
                fp,
                "Reduction",
                n,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * ---------------------------------------------
             * MASTER
             * ---------------------------------------------
             */
            start = omp_get_wtime();

            result =
                sum_master(a, n, threads);

            end = omp_get_wtime();

            parallel_time = end - start;

            if (result != expected)
            {
                printf("ERROR: Master result incorrect!\n");
                free(a);
                fclose(fp);
                return 1;
            }

            speedup = serial_time / parallel_time;
            efficiency =
                (speedup / threads) * 100.0;

            save_result(
                fp,
                "Master",
                n,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * ---------------------------------------------
             * LOCKS
             * ---------------------------------------------
             */
            start = omp_get_wtime();

            result =
                sum_locks(a, n, threads);

            end = omp_get_wtime();

            parallel_time = end - start;

            if (result != expected)
            {
                printf("ERROR: Locks result incorrect!\n");
                free(a);
                fclose(fp);
                return 1;
            }

            speedup = serial_time / parallel_time;
            efficiency =
                (speedup / threads) * 100.0;

            save_result(
                fp,
                "Locks",
                n,
                threads,
                serial_time,
                parallel_time
            );
        }

        /*
         * Free array
         */
        free(a);
    }

    /*
     * Close CSV
     */
    fclose(fp);

    printf("\n----------------------------------------\n");
    printf("All tests completed successfully.\n");
    printf("Results saved to: sum_openmp_results.csv\n");
    printf("----------------------------------------\n");

    return 0;
}
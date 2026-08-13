#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4
#define NUM_INTERVALS 5

/* ---------------------------------------------------------
   Serial Summation
   --------------------------------------------------------- */
long long serial_sum(long long start, long long end)
{
    long long sum = 0;

    for (long long i = start; i <= end; i++)
    {
        sum += i;
    }

    return sum;
}

/* ---------------------------------------------------------
   Parallel Summation using STATIC scheduling
   --------------------------------------------------------- */
long long parallel_static(
    long long start,
    long long end,
    int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for schedule(static) reduction(+:sum)
    for (long long i = start; i <= end; i++)
    {
        sum += i;
    }

    return sum;
}

/* ---------------------------------------------------------
   Parallel Summation using DYNAMIC scheduling
   --------------------------------------------------------- */
long long parallel_dynamic(
    long long start,
    long long end,
    int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for schedule(dynamic, 1000) reduction(+:sum)
    for (long long i = start; i <= end; i++)
    {
        sum += i;
    }

    return sum;
}

/* ---------------------------------------------------------
   Parallel Summation using GUIDED scheduling
   --------------------------------------------------------- */
long long parallel_guided(
    long long start,
    long long end,
    int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for schedule(guided, 1000) reduction(+:sum)
    for (long long i = start; i <= end; i++)
    {
        sum += i;
    }

    return sum;
}

/* ---------------------------------------------------------
   Parallel Summation using AUTO scheduling
   --------------------------------------------------------- */
long long parallel_auto(
    long long start,
    long long end,
    int threads)
{
    long long sum = 0;

    omp_set_num_threads(threads);

    #pragma omp parallel for schedule(auto) reduction(+:sum)
    for (long long i = start; i <= end; i++)
    {
        sum += i;
    }

    return sum;
}

/* ---------------------------------------------------------
   Save results to CSV
   --------------------------------------------------------- */
void save_result(
    FILE *fp,
    const char *schedule,
    long long interval,
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
        schedule,
        interval,
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
     * Different interval sizes.
     *
     * The interval is:
     *
     * 1 to interval
     */
    long long intervals[NUM_INTERVALS] = {
        1000000,
        5000000,
        10000000,
        20000000,
        50000000
    };

    /*
     * Number of threads.
     */
    int thread_counts[NUM_THREADS] = {
        1, 2, 3, 4
    };

    FILE *fp;

    /*
     * REQUIRED FIRST OUTPUT LINE
     */
    printf("Pragun Kakkar - 240968332\n");

    printf("\nOpenMP Summation of Integers over an Interval\n");
    printf("=============================================\n");

    printf("Scheduling strategies: Static, Dynamic, Guided, Auto\n");
    printf("Threads tested: 1, 2, 3, 4\n");

    /*
     * Open CSV file.
     */
    fp = fopen("schedule_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating CSV file.\n");
        return 1;
    }

    fprintf(
        fp,
        "Schedule,Interval,Threads,SerialTime,ParallelTime,Speedup,Efficiency\n"
    );

    /*
     * =====================================================
     * TEST DIFFERENT INTERVALS
     * =====================================================
     */
    for (int s = 0; s < NUM_INTERVALS; s++)
    {
        long long end = intervals[s];

        long long start_value = 1;

        /*
         * -----------------------------------------------
         * SERIAL EXECUTION
         * -----------------------------------------------
         */
        double start_time = omp_get_wtime();

        long long expected =
            serial_sum(start_value, end);

        double end_time = omp_get_wtime();

        double serial_time =
            end_time - start_time;

        printf(
            "\nInterval: [%lld, %lld] | Sum = %lld\n",
            start_value,
            end,
            expected
        );

        /*
         * =================================================
         * THREAD COUNTS
         * =================================================
         */
        for (int t = 0; t < NUM_THREADS; t++)
        {
            int threads = thread_counts[t];

            long long result;

            double parallel_time;

            /*
             * =============================================
             * STATIC
             * =============================================
             */
            start_time = omp_get_wtime();

            result =
                parallel_static(
                    start_value,
                    end,
                    threads
                );

            end_time = omp_get_wtime();

            parallel_time =
                end_time - start_time;

            if (result != expected)
            {
                printf("ERROR: Static result incorrect!\n");

                fclose(fp);
                return 1;
            }

            save_result(
                fp,
                "Static",
                end,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * =============================================
             * DYNAMIC
             * =============================================
             */
            start_time = omp_get_wtime();

            result =
                parallel_dynamic(
                    start_value,
                    end,
                    threads
                );

            end_time = omp_get_wtime();

            parallel_time =
                end_time - start_time;

            if (result != expected)
            {
                printf("ERROR: Dynamic result incorrect!\n");

                fclose(fp);
                return 1;
            }

            save_result(
                fp,
                "Dynamic",
                end,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * =============================================
             * GUIDED
             * =============================================
             */
            start_time = omp_get_wtime();

            result =
                parallel_guided(
                    start_value,
                    end,
                    threads
                );

            end_time = omp_get_wtime();

            parallel_time =
                end_time - start_time;

            if (result != expected)
            {
                printf("ERROR: Guided result incorrect!\n");

                fclose(fp);
                return 1;
            }

            save_result(
                fp,
                "Guided",
                end,
                threads,
                serial_time,
                parallel_time
            );

            /*
             * =============================================
             * AUTO
             * =============================================
             */
            start_time = omp_get_wtime();

            result =
                parallel_auto(
                    start_value,
                    end,
                    threads
                );

            end_time = omp_get_wtime();

            parallel_time =
                end_time - start_time;

            if (result != expected)
            {
                printf("ERROR: Auto result incorrect!\n");

                fclose(fp);
                return 1;
            }

            save_result(
                fp,
                "Auto",
                end,
                threads,
                serial_time,
                parallel_time
            );
        }
    }

    fclose(fp);

    printf("\n----------------------------------------\n");
    printf("All tests completed successfully.\n");
    printf("Results saved to: schedule_results.csv\n");
    printf("----------------------------------------\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_SIZES 5
#define NUM_THREAD_COUNTS 5

/* ---------------------------------------------------------
   Sequential Search - Serial Version
   --------------------------------------------------------- */
int sequential_search_serial(int a[], int n, int key)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (a[i] == key)
            return i;
    }

    return -1;
}

/* ---------------------------------------------------------
   Sequential Search - Parallel Version

   The array is divided among OpenMP threads.
   Each thread searches its assigned portion.

   reduction(min:result) ensures that the smallest
   index found is returned.
   --------------------------------------------------------- */
int sequential_search_parallel(int a[], int n, int key, int num_threads)
{
    int result = n;

    omp_set_num_threads(num_threads);

    #pragma omp parallel for schedule(static) reduction(min:result)
    for (int i = 0; i < n; i++)
    {
        if (a[i] == key)
        {
            if (i < result)
                result = i;
        }
    }

    if (result == n)
        return -1;

    return result;
}

/* ---------------------------------------------------------
   Generate array
   --------------------------------------------------------- */
void generate_array(int a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i] = i;
    }
}

/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */
int main()
{
    int sizes[NUM_SIZES] = {
        100000,
        500000,
        1000000,
        5000000,
        10000000
    };

    int thread_counts[NUM_THREAD_COUNTS] = {
        1, 2, 4, 8, 16
    };

    printf("Pragun Kakkar - 240968332\n");

    printf("\nParallel Sequential Search using OpenMP\n");
    printf("========================================\n");

    printf("\n%-12s %-8s %-15s %-15s %-12s %-15s\n",
           "Input Size",
           "Threads",
           "Serial(s)",
           "Parallel(s)",
           "Speedup",
           "Efficiency(%)");

    printf("------------------------------------------------------------------------\n");

    FILE *fp = fopen("sequential_search_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error opening CSV file.\n");
        return 1;
    }

    fprintf(fp,
            "InputSize,Threads,SerialTime,ParallelTime,Speedup,Efficiency\n");

    for (int s = 0; s < NUM_SIZES; s++)
    {
        int n = sizes[s];

        int *array = (int *)malloc(n * sizeof(int));

        if (array == NULL)
        {
            printf("Memory allocation failed for size %d.\n", n);
            fclose(fp);
            return 1;
        }

        /*
         * Create sorted array:
         *
         * 0, 1, 2, 3, ... n-1
         */
        generate_array(array, n);

        /*
         * Search for the LAST element.
         *
         * This forces the entire array to be searched
         * in the serial version.
         */
        int key = n - 1;

        /* -------------------------------
           Serial Search
           ------------------------------- */
        double start = omp_get_wtime();

        int serial_result =
            sequential_search_serial(array, n, key);

        double end = omp_get_wtime();

        double serial_time = end - start;

        /*
         * Verify serial result
         */
        if (serial_result != key)
        {
            printf("Serial search failed!\n");
            free(array);
            fclose(fp);
            return 1;
        }

        /* -------------------------------
           Parallel Search
           ------------------------------- */
        for (int t = 0; t < NUM_THREAD_COUNTS; t++)
        {
            int threads = thread_counts[t];

            start = omp_get_wtime();

            int parallel_result =
                sequential_search_parallel(
                    array,
                    n,
                    key,
                    threads
                );

            end = omp_get_wtime();

            double parallel_time = end - start;

            /*
             * Verify parallel result
             */
            if (parallel_result != key)
            {
                printf("Parallel search failed for %d threads!\n",
                       threads);

                free(array);
                fclose(fp);
                return 1;
            }

            /*
             * Calculate speedup
             *
             * Speedup = Serial Time / Parallel Time
             */
            double speedup =
                serial_time / parallel_time;

            /*
             * Calculate efficiency
             *
             * Efficiency = Speedup / Number of Threads
             *              × 100
             */
            double efficiency =
                (speedup / threads) * 100.0;

            printf("%-12d %-8d %-15.6f %-15.6f %-12.3f %-15.2f\n",
                   n,
                   threads,
                   serial_time,
                   parallel_time,
                   speedup,
                   efficiency);

            fprintf(fp,
                    "%d,%d,%.8f,%.8f,%.6f,%.4f\n",
                    n,
                    threads,
                    serial_time,
                    parallel_time,
                    speedup,
                    efficiency);
        }

        free(array);
    }

    fclose(fp);

    printf("\nResults saved to: sequential_search_results.csv\n");

    return 0;
}
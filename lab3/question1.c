#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NUM_SIZES 5
#define NUM_THREAD_COUNTS 5

/* ---------------------------------------------------------
   Serial Selection Sort
   --------------------------------------------------------- */
void selection_sort_serial(int a[], int n)
{
    int i, j, min_idx, temp;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;

        for (j = i + 1; j < n; j++)
        {
            if (a[j] < a[min_idx])
                min_idx = j;
        }

        temp = a[i];
        a[i] = a[min_idx];
        a[min_idx] = temp;
    }
}

/* ---------------------------------------------------------
   Parallel Selection Sort using OpenMP
   --------------------------------------------------------- */
void selection_sort_parallel(int a[], int n, int num_threads)
{
    int i;

    omp_set_num_threads(num_threads);

    for (i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        int min_value = a[i];

        /*
         * Each thread finds its local minimum.
         */
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            int total_threads = omp_get_num_threads();

            int local_min_idx = i;
            int local_min_value = a[i];

            /*
             * IMPORTANT:
             * j is declared INSIDE the parallel block,
             * therefore every thread has its own j.
             */
            int j;

            /*
             * Divide the remaining array among threads.
             */
            int remaining = n - (i + 1);
            int chunk = (remaining + total_threads - 1)
                        / total_threads;

            int start = i + 1 + tid * chunk;
            int end = start + chunk;

            if (end > n)
                end = n;

            if (start < n)
            {
                for (j = start; j < end; j++)
                {
                    if (a[j] < local_min_value)
                    {
                        local_min_value = a[j];
                        local_min_idx = j;
                    }
                }
            }

            /*
             * Safely compare each thread's local minimum
             * with the global minimum.
             */
            #pragma omp critical
            {
                if (local_min_value < min_value)
                {
                    min_value = local_min_value;
                    min_idx = local_min_idx;
                }
            }
        }

        /*
         * Swap the global minimum with the current position.
         */
        if (min_idx != i)
        {
            int temp = a[i];
            a[i] = a[min_idx];
            a[min_idx] = temp;
        }
    }
}

/* ---------------------------------------------------------
   Check whether array is sorted
   --------------------------------------------------------- */
int is_sorted(int a[], int n)
{
    int i;

    for (i = 0; i < n - 1; i++)
    {
        if (a[i] > a[i + 1])
            return 0;
    }

    return 1;
}

/* ---------------------------------------------------------
   Generate random array
   --------------------------------------------------------- */
void generate_array(int a[], int n)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = rand() % 100000;
}

/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */
int main()
{
    int sizes[NUM_SIZES] = {
        1000,
        2000,
        4000,
        8000,
        12000
    };

    int thread_counts[NUM_THREAD_COUNTS] = {
        1, 2, 4, 8, 16
    };

    FILE *fp;

    printf("Pragun Kakkar - 240968332\n");

    printf("\nParallel Selection Sort using OpenMP\n");
    printf("====================================\n");

    printf("\n%-10s %-8s %-15s %-15s %-12s %-15s\n",
           "Size",
           "Threads",
           "Serial(s)",
           "Parallel(s)",
           "Speedup",
           "Efficiency(%)");

    printf("------------------------------------------------------------------------\n");

    fp = fopen("selection_sort_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error opening CSV file.\n");
        return 1;
    }

    fprintf(fp,
            "InputSize,Threads,SerialTime,ParallelTime,Speedup,Efficiency\n");

    srand(42);

    for (int s = 0; s < NUM_SIZES; s++)
    {
        int n = sizes[s];

        int *original = malloc(n * sizeof(int));
        int *serial_array = malloc(n * sizeof(int));
        int *parallel_array = malloc(n * sizeof(int));

        if (original == NULL ||
            serial_array == NULL ||
            parallel_array == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }

        generate_array(original, n);

        /*
         * -------------------------
         * SERIAL SORT
         * -------------------------
         */
        memcpy(serial_array,
               original,
               n * sizeof(int));

        double start = omp_get_wtime();

        selection_sort_serial(serial_array, n);

        double end = omp_get_wtime();

        double serial_time = end - start;

        if (!is_sorted(serial_array, n))
        {
            printf("Serial sorting failed!\n");
            return 1;
        }

        /*
         * -------------------------
         * PARALLEL SORT
         * -------------------------
         */
        for (int t = 0; t < NUM_THREAD_COUNTS; t++)
        {
            int threads = thread_counts[t];

            memcpy(parallel_array,
                   original,
                   n * sizeof(int));

            start = omp_get_wtime();

            selection_sort_parallel(
                parallel_array,
                n,
                threads
            );

            end = omp_get_wtime();

            double parallel_time = end - start;

            if (!is_sorted(parallel_array, n))
            {
                printf("Parallel sorting failed for %d threads!\n",
                       threads);

                free(original);
                free(serial_array);
                free(parallel_array);
                fclose(fp);

                return 1;
            }

            double speedup =
                serial_time / parallel_time;

            double efficiency =
                (speedup / threads) * 100.0;

            printf("%-10d %-8d %-15.6f %-15.6f %-12.3f %-15.2f\n",
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

        free(original);
        free(serial_array);
        free(parallel_array);
    }

    fclose(fp);

    printf("\nResults saved to selection_sort_results.csv\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_SIZES 5
#define NUM_THREAD_COUNTS 5

/* ---------------------------------------------------------
   Serial Vector Operations
   --------------------------------------------------------- */

void vector_add_serial(double *A, double *B, double *C, int n)
{
    for (int i = 0; i < n; i++)
        C[i] = A[i] + B[i];
}

void vector_sub_serial(double *A, double *B, double *D, int n)
{
    for (int i = 0; i < n; i++)
        D[i] = A[i] - B[i];
}

void vector_mul_serial(double *A, double *B, double *E, int n)
{
    for (int i = 0; i < n; i++)
        E[i] = A[i] * B[i];
}

/* ---------------------------------------------------------
   Parallel Vector Operations using TASK-LEVEL PARALLELISM

   Three independent tasks are created:

       Task 1 -> Vector Addition
       Task 2 -> Vector Subtraction
       Task 3 -> Vector Multiplication

   Since these operations are independent, different
   OpenMP threads can execute them simultaneously.
   --------------------------------------------------------- */

void vector_operations_parallel(
    double *A,
    double *B,
    double *C,
    double *D,
    double *E,
    int n,
    int num_threads)
{
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        #pragma omp single
        {
            /*
             * TASK 1: Vector Addition
             */
            #pragma omp task
            {
                for (int i = 0; i < n; i++)
                    C[i] = A[i] + B[i];
            }

            /*
             * TASK 2: Vector Subtraction
             */
            #pragma omp task
            {
                for (int i = 0; i < n; i++)
                    D[i] = A[i] - B[i];
            }

            /*
             * TASK 3: Vector Multiplication
             */
            #pragma omp task
            {
                for (int i = 0; i < n; i++)
                    E[i] = A[i] * B[i];
            }

            /*
             * Wait until all three tasks finish.
             */
            #pragma omp taskwait
        }
    }
}

/* ---------------------------------------------------------
   Verify results
   --------------------------------------------------------- */

int verify_results(
    double *A,
    double *B,
    double *C,
    double *D,
    double *E,
    int n)
{
    for (int i = 0; i < n; i++)
    {
        if (C[i] != A[i] + B[i])
            return 0;

        if (D[i] != A[i] - B[i])
            return 0;

        if (E[i] != A[i] * B[i])
            return 0;
    }

    return 1;
}

/* ---------------------------------------------------------
   Initialize vectors
   --------------------------------------------------------- */

void initialize_vectors(double *A, double *B, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = (double)(i + 1);
        B[i] = (double)(2 * (i + 1));
    }
}

/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */

int main()
{
    /*
     * Different input sizes
     */
    int sizes[NUM_SIZES] = {
        1000000,
        5000000,
        10000000,
        20000000,
        50000000
    };

    /*
     * Number of threads to test.
     *
     * There are only 3 independent tasks, so maximum
     * useful task-level parallelism is approximately
     * 3 threads.
     */
    int thread_counts[NUM_THREAD_COUNTS] = {
        1, 2, 3, 4, 8
    };

    FILE *fp;

    /*
     * REQUIRED FIRST OUTPUT LINE
     */
    printf("Pragun Kakkar - 240968332\n");

    printf("\nOpenMP Task-Level Parallelism");
    printf("\nVector Addition, Subtraction and Multiplication");
    printf("\n===============================================\n");

    printf("\n%-12s %-8s %-15s %-15s %-12s %-15s\n",
           "Input Size",
           "Threads",
           "Serial(s)",
           "Parallel(s)",
           "Speedup",
           "Efficiency(%)");

    printf("------------------------------------------------------------------------\n");

    /*
     * Open CSV file
     */
    fp = fopen("vector_task_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating CSV file.\n");
        return 1;
    }

    fprintf(fp,
            "InputSize,Threads,SerialTime,ParallelTime,Speedup,Efficiency\n");

    /*
     * Test each input size
     */
    for (int s = 0; s < NUM_SIZES; s++)
    {
        int n = sizes[s];

        printf("\nTesting input size: %d\n", n);

        /*
         * Allocate vectors
         */
        double *A = (double *)malloc(n * sizeof(double));
        double *B = (double *)malloc(n * sizeof(double));

        double *C_serial = (double *)malloc(n * sizeof(double));
        double *D_serial = (double *)malloc(n * sizeof(double));
        double *E_serial = (double *)malloc(n * sizeof(double));

        double *C_parallel = (double *)malloc(n * sizeof(double));
        double *D_parallel = (double *)malloc(n * sizeof(double));
        double *E_parallel = (double *)malloc(n * sizeof(double));

        if (A == NULL || B == NULL ||
            C_serial == NULL || D_serial == NULL ||
            E_serial == NULL ||
            C_parallel == NULL || D_parallel == NULL ||
            E_parallel == NULL)
        {
            printf("Memory allocation failed!\n");

            fclose(fp);
            return 1;
        }

        /*
         * Initialize input vectors
         */
        initialize_vectors(A, B, n);

        /* =================================================
           SERIAL EXECUTION
           ================================================= */

        double start = omp_get_wtime();

        /*
         * All three operations execute sequentially.
         */
        vector_add_serial(
            A, B, C_serial, n
        );

        vector_sub_serial(
            A, B, D_serial, n
        );

        vector_mul_serial(
            A, B, E_serial, n
        );

        double end = omp_get_wtime();

        double serial_time = end - start;

        /*
         * Verify serial result
         */
        if (!verify_results(
                A, B,
                C_serial,
                D_serial,
                E_serial,
                n))
        {
            printf("Serial result verification failed!\n");

            fclose(fp);
            return 1;
        }

        /* =================================================
           PARALLEL EXECUTION
           ================================================= */

        for (int t = 0; t < NUM_THREAD_COUNTS; t++)
        {
            int threads = thread_counts[t];

            start = omp_get_wtime();

            /*
             * Three vector operations are submitted
             * as independent OpenMP tasks.
             */
            vector_operations_parallel(
                A,
                B,
                C_parallel,
                D_parallel,
                E_parallel,
                n,
                threads
            );

            end = omp_get_wtime();

            double parallel_time = end - start;

            /*
             * Verify parallel result
             */
            if (!verify_results(
                    A, B,
                    C_parallel,
                    D_parallel,
                    E_parallel,
                    n))
            {
                printf(
                    "Parallel result verification failed "
                    "for %d threads!\n",
                    threads
                );

                fclose(fp);
                return 1;
            }

            /*
             * Speedup
             *
             * S = Tserial / Tparallel
             */
            double speedup =
                serial_time / parallel_time;

            /*
             * Efficiency
             *
             * E = Speedup / Number of Threads * 100
             */
            double efficiency =
                (speedup / threads) * 100.0;

            printf(
                "%-12d %-8d %-15.6f %-15.6f %-12.3f %-15.2f\n",
                n,
                threads,
                serial_time,
                parallel_time,
                speedup,
                efficiency
            );

            /*
             * Save to CSV
             */
            fprintf(
                fp,
                "%d,%d,%.8f,%.8f,%.6f,%.4f\n",
                n,
                threads,
                serial_time,
                parallel_time,
                speedup,
                efficiency
            );
        }

        /*
         * Free memory
         */
        free(A);
        free(B);

        free(C_serial);
        free(D_serial);
        free(E_serial);

        free(C_parallel);
        free(D_parallel);
        free(E_parallel);
    }

    fclose(fp);

    printf("\nResults saved to: vector_task_results.csv\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NUM_SIZES 5
#define NUM_THREADS 4

/* ---------------------------------------------------------
   Serial Monte Carlo computation of PI
   --------------------------------------------------------- */
long long serial_pi(long long n, unsigned int seed)
{
    long long inside = 0;

    unsigned int local_seed = seed;

    for (long long i = 0; i < n; i++)
    {
        double x =
            (double)rand_r(&local_seed) / RAND_MAX;

        double y =
            (double)rand_r(&local_seed) / RAND_MAX;

        /*
         * Check whether point lies inside
         * the quarter of the unit circle.
         */
        if ((x * x + y * y) <= 1.0)
        {
            inside++;
        }
    }

    return inside;
}

/* ---------------------------------------------------------
   Parallel Monte Carlo computation of PI
   --------------------------------------------------------- */
long long parallel_pi(
    long long n,
    int threads)
{
    long long inside = 0;

    omp_set_num_threads(threads);

    /*
     * Each thread has its own random seed.
     */
    #pragma omp parallel reduction(+:inside)
    {
        int tid = omp_get_thread_num();

        /*
         * Different seed for every thread.
         */
        unsigned int seed =
            (unsigned int)time(NULL)
            ^ (unsigned int)(tid * 1234567 + 987654);

        /*
         * Parallelize the generation of points.
         */
        #pragma omp for
        for (long long i = 0; i < n; i++)
        {
            double x =
                (double)rand_r(&seed) / RAND_MAX;

            double y =
                (double)rand_r(&seed) / RAND_MAX;

            /*
             * Point is inside quarter circle.
             */
            if ((x * x + y * y) <= 1.0)
            {
                inside++;
            }
        }
    }

    return inside;
}

/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */
int main()
{
    /*
     * Number of random points.
     */
    long long points[NUM_SIZES] = {
        1000000,
        5000000,
        10000000,
        20000000,
        50000000
    };

    /*
     * Threads to test.
     */
    int thread_counts[NUM_THREADS] = {
        1, 2, 3, 4
    };

    FILE *fp;

    /*
     * REQUIRED FIRST OUTPUT LINE
     */
    printf("Pragun Kakkar - 240968332\n");

    printf("\nMonte Carlo Computation of PI using OpenMP\n");
    printf("==========================================\n");

    printf("Threads tested: 1, 2, 3, 4\n");

    /*
     * Open CSV file.
     */
    fp = fopen("pi_results.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating CSV file.\n");
        return 1;
    }

    fprintf(
        fp,
        "Points,Threads,SerialTime,ParallelTime,PI,Speedup,Efficiency\n"
    );

    /*
     * =====================================================
     * Test different numbers of points
     * =====================================================
     */
    for (int s = 0; s < NUM_SIZES; s++)
    {
        long long n = points[s];

        /*
         * -------------------------------------------------
         * SERIAL EXECUTION
         * -------------------------------------------------
         */

        double start = omp_get_wtime();

        long long serial_inside =
            serial_pi(
                n,
                (unsigned int)time(NULL)
            );

        double end = omp_get_wtime();

        double serial_time =
            end - start;

        /*
         * Calculate serial PI.
         */
        double serial_pi_value =
            4.0 * (double)serial_inside / (double)n;

        printf(
            "\nPoints = %lld | Serial PI = %.8f\n",
            n,
            serial_pi_value
        );

        /*
         * -------------------------------------------------
         * PARALLEL EXECUTION
         * -------------------------------------------------
         */
        for (int t = 0; t < NUM_THREADS; t++)
        {
            int threads = thread_counts[t];

            start = omp_get_wtime();

            long long parallel_inside =
                parallel_pi(
                    n,
                    threads
                );

            end = omp_get_wtime();

            double parallel_time =
                end - start;

            /*
             * Calculate PI.
             */
            double pi_value =
                4.0 *
                (double)parallel_inside /
                (double)n;

            /*
             * Calculate speedup.
             */
            double speedup =
                serial_time /
                parallel_time;

            /*
             * Calculate efficiency.
             */
            double efficiency =
                (speedup / threads) * 100.0;

            /*
             * Display concise output.
             */
            printf(
                "Threads = %d | PI = %.8f | Time = %.6f s | "
                "Speedup = %.3f | Efficiency = %.2f%%\n",
                threads,
                pi_value,
                parallel_time,
                speedup,
                efficiency
            );

            /*
             * Save results to CSV.
             */
            fprintf(
                fp,
                "%lld,%d,%.8f,%.8f,%.8f,%.6f,%.4f\n",
                n,
                threads,
                serial_time,
                parallel_time,
                pi_value,
                speedup,
                efficiency
            );
        }
    }

    fclose(fp);

    printf("\n------------------------------------------\n");
    printf("Computation completed successfully.\n");
    printf("Results saved to: pi_results.csv\n");
    printf("------------------------------------------\n");

    return 0;
}
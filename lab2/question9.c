#include <stdio.h>
#include <omp.h>

#define SIZE 5

int main()
{
    int A[SIZE][SIZE], B[SIZE][SIZE];
    int i, j;
    printf("Pragun Kakkar - 240968332\n");

    printf("Enter 5x5 Matrix A:\n");

    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            scanf("%d",&A[i][j]);
        }
    }

    int maxRow[SIZE], minRow[SIZE];

    // Find maximum and minimum values of each row
    for(i = 0; i < SIZE; i++)
    {
        maxRow[i] = A[i][0];
        minRow[i] = A[i][0];

        for(j = 1; j < SIZE; j++)
        {
            if(A[i][j] > maxRow[i])
                maxRow[i] = A[i][j];

            if(A[i][j] < minRow[i])
                minRow[i] = A[i][j];
        }
    }


    double serial_start, serial_end;
    double parallel_start, parallel_end;

    double serial_time, parallel_time;


    // ---------------- Serial Execution ----------------

    serial_start = omp_get_wtime();

    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(i == j)
                B[i][j] = 0;

            else if(i > j)
                B[i][j] = maxRow[i];

            else
                B[i][j] = minRow[i];
        }
    }

    serial_end = omp_get_wtime();

    serial_time = serial_end - serial_start;


    printf("\nSerial Execution Time = %lf seconds\n",serial_time);



    // ---------------- Parallel Execution ----------------


    int threads[] = {2,4,6,8};


    printf("\nThreads\tTime\t\tSpeedup\tEfficiency\n");
    printf("-----------------------------------------------\n");


    for(int t = 0; t < 4; t++)
    {
        omp_set_num_threads(threads[t]);


        parallel_start = omp_get_wtime();


        #pragma omp parallel for private(j)
        for(i = 0; i < SIZE; i++)
        {
            for(j = 0; j < SIZE; j++)
            {
                if(i == j)
                    B[i][j] = 0;

                else if(i > j)
                    B[i][j] = maxRow[i];

                else
                    B[i][j] = minRow[i];
            }
        }


        parallel_end = omp_get_wtime();


        parallel_time = parallel_end - parallel_start;


        double speedup = serial_time / parallel_time;

        double efficiency = speedup / threads[t];


        printf("%d\t%lf\t%lf\t%lf\n",
               threads[t],
               parallel_time,
               speedup,
               efficiency);
    }



    printf("\nResultant Matrix B:\n");

    for(i=0;i<SIZE;i++)
    {
        for(j=0;j<SIZE;j++)
        {
            printf("%d\t",B[i][j]);
        }
        printf("\n");
    }


    return 0;
}
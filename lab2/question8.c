#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int N;

    printf("Pragun kakkar - 240968332\n");
    printf("Enter the size of square matrix (N x N): ");
    scanf("%d",&N);

    int **A;
    int *X,*Y;

    A=(int **)malloc(N*sizeof(int *));
    for(int i=0;i<N;i++)
        A[i]=(int *)malloc(N*sizeof(int));

    X=(int *)malloc(N*sizeof(int));
    Y=(int *)malloc(N*sizeof(int));

    // Initialize Matrix and Vector
    for(int i=0;i<N;i++)
    {
        X[i]=1;

        for(int j=0;j<N;j++)
            A[i][j]=1;
    }

    int thread_count[]={2,4,6,8};

    // ---------------- Serial Execution ----------------

    double start=omp_get_wtime();

    for(int i=0;i<N;i++)
    {
        Y[i]=0;

        for(int j=0;j<N;j++)
            Y[i]+=A[i][j]*X[j];
    }

    double end=omp_get_wtime();

    double serial_time=end-start;

    printf("\nSerial Time = %lf sec\n",serial_time);

    printf("\nThreads\tTime\t\tSpeedup\tEfficiency\n");

    // ---------------- Parallel Execution ----------------

    for(int t=0;t<4;t++)
    {
        omp_set_num_threads(thread_count[t]);

        start=omp_get_wtime();

        #pragma omp parallel for
        for(int i=0;i<N;i++)
        {
            Y[i]=0;

            for(int j=0;j<N;j++)
                Y[i]+=A[i][j]*X[j];
        }

        end=omp_get_wtime();

        double parallel_time=end-start;

        double speedup=serial_time/parallel_time;

        double efficiency=speedup/thread_count[t];

        printf("%d\t%lf\t%lf\t%lf\n",
                thread_count[t],
                parallel_time,
                speedup,
                efficiency);
    }

    for(int i=0;i<N;i++)
        free(A[i]);

    free(A);
    free(X);
    free(Y);

    return 0;
}
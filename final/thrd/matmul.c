#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define M 1000
#define N 1000
#define P 1000

int A[M][N], B[N][P], C[M][P];

struct Data {
    int row;
};

void *multiply(void *arg) {
    struct timeval start, end;
    struct Data *d = (struct Data *)arg;

    gettimeofday(&start, NULL);

    int i = d->row;

    for (int j = 0; j < P; j++) {
        C[i][j] = 0;

        for (int k = 0; k < N; k++) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }

    gettimeofday(&end, NULL);

    double time = (end.tv_sec - start.tv_sec) * 1000000.0 +
                  (end.tv_usec - start.tv_usec);

    printf("Thread calculating row %d: %.2f microseconds\n",
           i, time);

    return NULL;
}

int main() {
    pthread_t threads[M];
    struct Data data[M];

    struct timeval start, end;

    // Initialize matrices
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++)
            A[i][j] = i + j + 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++)
            B[i][j] = i + j + 1;
    }

    // Sequential multiplication
    gettimeofday(&start, NULL);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;

            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }

    gettimeofday(&end, NULL);

    double seqTime = (end.tv_sec - start.tv_sec) * 1000000.0 +
                     (end.tv_usec - start.tv_usec);

    printf("Sequential time: %.2f microseconds\n", seqTime);

    // Parallel multiplication
    gettimeofday(&start, NULL);

    for (int i = 0; i < M; i++) {
        data[i].row = i;
        pthread_create(&threads[i], NULL, multiply, &data[i]);
    }

    for (int i = 0; i < M; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);

    double parTime = (end.tv_sec - start.tv_sec) * 1000000.0 +
                     (end.tv_usec - start.tv_usec);

    printf("Parallel time: %.2f microseconds\n", parTime);

    // Print result
/*    printf("\nResult Matrix:\n");

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++)
            printf("%d ", C[i][j]);

        printf("\n");
    }
*/
    return 0;
}

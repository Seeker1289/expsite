// Parallel matrix multiply C(MxP) = A(MxN) * B(NxP).  Usage: ./matmul M N P [threads]
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
static double *A, *B, *Cs, *Cp; static int M, N, P;
typedef struct { int id, r0, r1; double t; } Arg;
static double now() { struct timeval tv; gettimeofday(&tv, NULL); return tv.tv_sec + tv.tv_usec / 1e6; }
static void rows(double *C, int r0, int r1) {
    for (int i = r0; i < r1; i++)
        for (int k = 0; k < N; k++) {
            double a = A[i * N + k];
            for (int j = 0; j < P; j++) C[i * P + j] += a * B[k * P + j];
        }
}
static void *worker(void *p) {
    Arg *a = p; double s = now();
    rows(Cp, a->r0, a->r1);                      // each thread owns a block of rows: no locking needed
    a->t = now() - s; return NULL;
}
int main(int argc, char **argv) {
    M = argc > 1 ? atoi(argv[1]) : 500; N = argc > 2 ? atoi(argv[2]) : 500; P = argc > 3 ? atoi(argv[3]) : 500;
    int T = argc > 4 ? atoi(argv[4]) : 4;
    A = malloc(sizeof(double) * M * N); B = malloc(sizeof(double) * N * P);
    Cs = calloc(M * P, sizeof(double)); Cp = calloc(M * P, sizeof(double));
    for (int i = 0; i < M * N; i++) A[i] = rand() % 10;
    for (int i = 0; i < N * P; i++) B[i] = rand() % 10;
    double s = now(); rows(Cs, 0, M); double tseq = now() - s;
    pthread_t th[T]; Arg ar[T];
    s = now();
    for (int i = 0; i < T; i++) {
        ar[i] = (Arg){ i, i * M / T, (i + 1) * M / T, 0 };
        pthread_create(&th[i], NULL, worker, &ar[i]);
    }
    for (int i = 0; i < T; i++) pthread_join(th[i], NULL);
    double tpar = now() - s;
    for (int i = 0; i < T; i++) printf("Thread %d (rows %d-%d): %.4f s\n", i, ar[i].r0, ar[i].r1 - 1, ar[i].t);
    printf("Sequential: %.4f s | Parallel(%d threads): %.4f s | Speedup %.2fx | Results %s\n",
           tseq, T, tpar, tseq / tpar, memcmp(Cs, Cp, sizeof(double) * M * P) == 0 ? "MATCH" : "DIFFER");
    return 0;
}

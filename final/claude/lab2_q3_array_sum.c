// Sum an array with T threads; mutex protects the global result.  Usage: ./arraysum [N] [T]
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
static int *a; static long N; static long long total = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
typedef struct { long lo, hi; } Arg;
static void *worker(void *p) {
    Arg *g = p; long long local = 0;
    for (long i = g->lo; i < g->hi; i++) local += a[i];   // private partial sum: no lock needed
    pthread_mutex_lock(&m);                                // critical section: update shared total
    total += local;
    pthread_mutex_unlock(&m);
    return NULL;
}
int main(int argc, char **argv) {
    N = argc > 1 ? atol(argv[1]) : 1000000; int T = argc > 2 ? atoi(argv[2]) : 4;
    a = malloc(N * sizeof(int)); long long expect = 0;
    for (long i = 0; i < N; i++) { a[i] = rand() % 100; expect += a[i]; }
    pthread_t th[T]; Arg ar[T];
    for (int i = 0; i < T; i++) { ar[i] = (Arg){ i * N / T, (i + 1) * N / T }; pthread_create(&th[i], NULL, worker, &ar[i]); }
    for (int i = 0; i < T; i++) pthread_join(th[i], NULL);
    printf("Threaded sum = %lld, serial check = %lld -> %s\n", total, expect, total == expect ? "OK" : "WRONG");
    return 0;
}

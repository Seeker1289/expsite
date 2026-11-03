// Multithreaded merge sort: every recursive call spawns 2 threads. Usage: ./msort [N] [cutoff]
// cutoff: below this size sort sequentially (default 1 = a thread for every call, as in the exercise)
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
static int *a, *tmp, cutoff = 1;
static volatile long nthreads = 0;
typedef struct { int lo, hi; } Arg;
static void merge(int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    memcpy(a + lo, tmp + lo, (hi - lo) * sizeof(int));
}
static void *msort(void *p) {
    Arg *g = p; int lo = g->lo, hi = g->hi;
    if (hi - lo < 2) return NULL;
    int mid = (lo + hi) / 2;
    Arg l = {lo, mid}, r = {mid, hi};
    if (hi - lo > cutoff) {
        pthread_attr_t at; pthread_attr_init(&at); pthread_attr_setstacksize(&at, 64 * 1024);
        pthread_t tl, tr;
        int ok1 = pthread_create(&tl, &at, msort, &l) == 0;
        int ok2 = pthread_create(&tr, &at, msort, &r) == 0;
        __sync_fetch_and_add(&nthreads, ok1 + ok2);
        if (!ok1) msort(&l);               // fall back to running in this thread if creation fails
        if (!ok2) msort(&r);
        if (ok1) pthread_join(tl, NULL);
        if (ok2) pthread_join(tr, NULL);
    } else { msort(&l); msort(&r); }       // plain recursion (not counted as thread)
    merge(lo, mid, hi);                    // parent merges the two sorted halves
    return NULL;
}
int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 1000; if (argc > 2) cutoff = atoi(argv[2]);
    a = malloc(n * sizeof(int)); tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = rand() % 100000;
    struct timeval s, e; gettimeofday(&s, NULL);
    Arg g = {0, n}; msort(&g);
    gettimeofday(&e, NULL);
    int ok = 1; for (int i = 1; i < n; i++) if (a[i-1] > a[i]) ok = 0;
    printf("n=%d cutoff=%d threads=%ld time=%.4f s sorted=%s\n", n, cutoff, nthreads,
           (e.tv_sec - s.tv_sec) + (e.tv_usec - s.tv_usec) / 1e6, ok ? "yes" : "NO");
    return 0;
}

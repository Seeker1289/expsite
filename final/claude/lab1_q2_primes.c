// Parallel prime generation.  Usage: ./primes N [mode]
//   mode 0: fixed-count, UNORDERED   (state var + mutex + condvar; fast, order may be off)
//   mode 1: ORDERED via one lock held across "find prime + report" (atomic, but serial)
//   mode 2: ORDERED but parallel: test in parallel, only *commit* in candidate order
// Compile: gcc -O2 -pthread primes.c -o primes   (add -DOUTPUT=0 to disable printing)
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef OUTPUT
#define OUTPUT 1
#endif
#define NTHREADS 8
static long target, next_num = 2, found = 0, turn = 2;
static int done = 0, mode = 0;
static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t done_cv = PTHREAD_COND_INITIALIZER, turn_cv = PTHREAD_COND_INITIALIZER;

static int is_prime(long n) {
    if (n < 2) return 0;
    for (long i = 2; i * i <= n; i++) if (n % i == 0) return 0;
    return 1;
}
// call with lk held
static void report(long n, long id) {
    found++;
#if OUTPUT
    printf("\033[%ldm%ld,\033[0m", 31 + id % 6, n);      // colour per thread
#endif
    if (found == target) { done = 1; pthread_cond_signal(&done_cv); }
}
static void *worker(void *arg) {
    long id = (long)arg;
    for (;;) {
        pthread_mutex_lock(&lk);
        if (done) { pthread_mutex_unlock(&lk); break; }
        long n = next_num++;
        if (mode == 1) {                                  // find + report atomically
            if (is_prime(n)) report(n, id);
            pthread_mutex_unlock(&lk);
            continue;
        }
        pthread_mutex_unlock(&lk);
        int p = is_prime(n);                              // heavy work outside the lock
        pthread_mutex_lock(&lk);
        if (mode == 2) while (turn != n) pthread_cond_wait(&turn_cv, &lk);
        if (p && !done) report(n, id);                    // check+count under lock => exactly N primes
        if (mode == 2) { turn++; pthread_cond_broadcast(&turn_cv); }
        pthread_mutex_unlock(&lk);
    }
    return NULL;
}
int main(int argc, char **argv) {
    target = argc > 1 ? atol(argv[1]) : 10;
    mode = argc > 2 ? atoi(argv[2]) : 0;
    pthread_t t[NTHREADS];
    for (long i = 0; i < NTHREADS; i++) pthread_create(&t[i], NULL, worker, (void *)i);
    pthread_mutex_lock(&lk);
    while (!done) pthread_cond_wait(&done_cv, &lk);       // parent sleeps until nth prime found
    pthread_mutex_unlock(&lk);
    printf("\n");
    exit(0);                                              // no pthread_join needed
}

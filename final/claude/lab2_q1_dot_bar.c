// main creates 2 threads: one prints '.', one prints '|'.  Usage: ./dotbar [N] [prio]
// "prio" gives the '|' thread a higher SCHED_FIFO priority (needs root: sudo ./dotbar 200 prio)
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
static int N = 200;
static void *printer(void *a) {
    char c = *(char *)a;
    for (int i = 0; i < N; i++) { putchar(c); fflush(stdout); }
    return NULL;
}
int main(int argc, char **argv) {
    if (argc > 1) N = atoi(argv[1]);
    int prio = argc > 2 && !strcmp(argv[2], "prio");
    char ch[2] = {'.', '|'};
    pthread_t t[2];
    for (int i = 0; i < 2; i++) {
        pthread_attr_t at; pthread_attr_init(&at);
        if (prio) {
            struct sched_param sp = { .sched_priority = i == 0 ? 10 : 50 };
            pthread_attr_setinheritsched(&at, PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&at, SCHED_FIFO);
            pthread_attr_setschedparam(&at, &sp);
        }
        int rc = pthread_create(&t[i], &at, printer, &ch[i]);
        if (rc) {   // usually EPERM when not root
            fprintf(stderr, "\n(priority not permitted: %s; using default)\n", strerror(rc));
            pthread_create(&t[i], NULL, printer, &ch[i]);
        }
    }
    pthread_join(t[0], NULL); pthread_join(t[1], NULL);
    printf("\n");
    return 0;
}

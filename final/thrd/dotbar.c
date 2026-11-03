#include <stdio.h>
#include <pthread.h>

#define N 100

void *printDots(void *arg) {
    for (int i = 0; i < N; i++) {
        printf(".");
        fflush(stdout);
    }
    return NULL;
}

void *printBars(void *arg) {
    for (int i = 0; i < N; i++) {
        printf("|");
        fflush(stdout);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, printDots, NULL);
    pthread_create(&t2, NULL, printBars, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\n");
    return 0;
}

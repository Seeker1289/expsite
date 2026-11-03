// Producer/Consumer with a 5-element stack, one mutex + two condition variables
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 5
#define ITEMS 15            // items produced/consumed in total (demo ends)
int stack[MAX], top = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER, not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *a) {
    for (int i = 0; i < ITEMS; i++) {
        pthread_mutex_lock(&m);
        while (top == MAX) {                       // while, not if: guards spurious wakeups
            printf("[P] Stack FULL, producer waiting...\n");
            pthread_cond_wait(&not_full, &m);      // releases mutex while sleeping
        }
        int v = rand() % 31;                       // 0..30
        stack[top++] = v;
        printf("[P] Pushed %d (size=%d)\n", v, top);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}
void *consumer(void *a) {
    sleep(1);                                      // let producer fill the stack first
    for (int i = 0; i < ITEMS; i++) {
        pthread_mutex_lock(&m);
        while (top == 0) {
            printf("[C] Stack EMPTY, consumer waiting...\n");
            pthread_cond_wait(&not_empty, &m);
        }
        int v = stack[--top];
        printf("[C] Popped %d (size=%d)\n", v, top);
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&m);
        usleep(200000);
    }
    return NULL;
}
int main() {
    srand(1);
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL); pthread_join(c, NULL);
    return 0;
}

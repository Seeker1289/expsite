#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define SIZE 5
#define TOTAL 20

int stack[SIZE];
int top = -1;
int produced = 0, consumed = 0;

pthread_mutex_t mutex;
pthread_cond_t notFull, notEmpty;

void *producer(void *arg) {
    while (produced < TOTAL) {
        pthread_mutex_lock(&mutex);

        while (top == SIZE - 1)
            pthread_cond_wait(&notFull, &mutex);

        int value = rand() % 31;

        stack[++top] = value;
        produced++;

        printf("Produced: %d\n", value);

        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumer(void *arg) {
    while (consumed < TOTAL) {
        pthread_mutex_lock(&mutex);

        while (top == -1)
            pthread_cond_wait(&notEmpty, &mutex);

        int value = stack[top--];
        consumed++;

        printf("Consumed: %d\n", value);

        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);

        usleep(100000);
    }

    return NULL;
}

int main() {
    pthread_t p, c;

    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);

    return 0;
}

#include <stdio.h>
#include <pthread.h>

#define SIZE 100
#define THREADS 4

int arr[SIZE];
int sum = 0;

pthread_mutex_t lock;

struct Data {
    int start;
    int end;
};

void *calculate(void *arg) {
    struct Data *d = (struct Data *)arg;

    int localSum = 0;

    // Calculate partial sum
    for (int i = d->start; i < d->end; i++) {
        localSum += arr[i];
    }

    // Critical section
    pthread_mutex_lock(&lock);

    sum += localSum;

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    struct Data data[THREADS];

    pthread_mutex_init(&lock, NULL);

    // Initialize array
    for (int i = 0; i < SIZE; i++)
        arr[i] = i + 1;

    // Divide array equally among threads
    for (int i = 0; i < THREADS; i++) {
        data[i].start = i * (SIZE / THREADS);
        data[i].end = (i + 1) * (SIZE / THREADS);

        pthread_create(&threads[i], NULL, calculate, &data[i]);
    }

    // Wait for all threads
    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Total sum = %d\n", sum);

    pthread_mutex_destroy(&lock);

    return 0;
}

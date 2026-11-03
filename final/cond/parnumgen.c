#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 8

int limit;
int count = 0;
int candidate = 2;
int stop = 0;

pthread_mutex_t mutex;
pthread_cond_t done;

int isPrime(int n) {
    if (n < 2)
        return 0;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

void *generate(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (stop) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        int num = candidate++;

        // Check and print atomically
        if (isPrime(num)) {
            printf("%d ", num);
            count++;

            if (count == limit) {
                stop = 1;
                pthread_cond_signal(&done);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./q2 <number_of_primes>\n");
        return 1;
    }

    limit = atoi(argv[1]);

    pthread_t threads[THREADS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&done, NULL);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, generate, NULL);
    }

    pthread_mutex_lock(&mutex);

    while (!stop)
        pthread_cond_wait(&done, &mutex);

    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("\nGenerated %d prime numbers.\n", count);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&done);

    return 0;
}

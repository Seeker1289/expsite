#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int readCount = 0;
int writeCount = 0;

sem_t resource;
sem_t readTry;
sem_t rmutex;
sem_t wmutex;

void *reader(void *arg) {
    int id = *(int *)arg;

    // Prevent readers from entering if writers are waiting
    sem_wait(&readTry);

    sem_wait(&rmutex);
    readCount++;

    if (readCount == 1)
        sem_wait(&resource);

    sem_post(&rmutex);
    sem_post(&readTry);

    printf("Reader %d is reading\n", id);
    sleep(2);
    printf("Reader %d finished reading\n", id);

    sem_wait(&rmutex);
    readCount--;

    if (readCount == 0)
        sem_post(&resource);

    sem_post(&rmutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    sem_wait(&wmutex);
    writeCount++;

    if (writeCount == 1)
        sem_wait(&readTry);

    sem_post(&wmutex);

    sem_wait(&resource);

    printf("Writer %d is writing\n", id);
    sleep(2);
    printf("Writer %d finished writing\n", id);

    sem_post(&resource);

    sem_wait(&wmutex);
    writeCount--;

    if (writeCount == 0)
        sem_post(&readTry);

    sem_post(&wmutex);

    return NULL;
}

int main() {
    pthread_t threads[9];

    int ids[9] = {1, 2, 3, 1, 4, 5, 2, 6, 7};

    int isWriter[9] = {
        0, 0, 0, 1, 0, 0, 1, 0, 0
    };

    int arrival[9] = {
        0, 2, 4, 5, 6, 7, 7, 8, 9
    };

    sem_init(&resource, 0, 1);
    sem_init(&readTry, 0, 1);
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);

    for (int i = 0; i < 9; i++) {
        sleep(i == 0 ? arrival[i] : arrival[i] - arrival[i-1]);

        if (isWriter[i])
            pthread_create(&threads[i], NULL, writer, &ids[i]);
        else
            pthread_create(&threads[i], NULL, reader, &ids[i]);
    }

    for (int i = 0; i < 9; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&resource);
    sem_destroy(&readTry);
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);

    return 0;
}

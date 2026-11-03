// Readers-Writers, WRITER preference, POSIX semaphores (Courtois et al. problem 2)
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
sem_t mutex1, mutex2, mutex3, r, w;   // mutex1: readcount, mutex2: writecount, mutex3+r: reader entry gate, w: resource
int readcount = 0, writecount = 0;
time_t t0;
typedef struct { const char *name; int arrive, exec; } Job;
#define LOG(j, msg) printf("t=%2ld  %-3s %s\n", time(NULL) - t0, j->name, msg), fflush(stdout)

void *reader(void *a) {
    Job *j = a; sleep(j->arrive); LOG(j, "arrives");
    sem_wait(&mutex3);              // only one reader queues on r at a time
    sem_wait(&r);                   // blocked here if any writer is waiting/writing
    sem_wait(&mutex1);
    if (++readcount == 1) sem_wait(&w);   // first reader locks out writers
    sem_post(&mutex1);
    sem_post(&r);
    sem_post(&mutex3);
    LOG(j, "START reading"); sleep(j->exec); LOG(j, "done reading");
    sem_wait(&mutex1);
    if (--readcount == 0) sem_post(&w);   // last reader lets writers in
    sem_post(&mutex1);
    return NULL;
}
void *writer(void *a) {
    Job *j = a; sleep(j->arrive); LOG(j, "arrives");
    sem_wait(&mutex2);
    if (++writecount == 1) sem_wait(&r);  // first waiting writer blocks NEW readers
    sem_post(&mutex2);
    sem_wait(&w);                         // exclusive access
    LOG(j, "START writing"); sleep(j->exec); LOG(j, "done writing");
    sem_post(&w);
    sem_wait(&mutex2);
    if (--writecount == 0) sem_post(&r);  // no writers left: readers allowed again
    sem_post(&mutex2);
    return NULL;
}
int main() {
    sem_init(&mutex1,0,1); sem_init(&mutex2,0,1); sem_init(&mutex3,0,1); sem_init(&r,0,1); sem_init(&w,0,1);
    Job jobs[] = {{"R1",0,8},{"R2",2,6},{"R3",4,5},{"W1",5,6},{"R4",6,2},{"R5",7,2},{"W2",7,4},{"R6",8,2},{"R7",9,3}};
    int n = sizeof jobs / sizeof jobs[0];
    pthread_t t[9]; t0 = time(NULL);
    for (int i = 0; i < n; i++) pthread_create(&t[i], NULL, jobs[i].name[0]=='R' ? reader : writer, &jobs[i]);
    for (int i = 0; i < n; i++) pthread_join(t[i], NULL);
    return 0;
}

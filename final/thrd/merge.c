#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 16
#define MAX_DEPTH 3

int arr[SIZE];
int threadCount = 0;

pthread_mutex_t lock;

struct Data {
    int left;
    int right;
    int depth;
};

void merge(int l, int mid, int r) {
    int temp[SIZE];
    int i = l, j = mid + 1, k = l;

    while (i <= mid && j <= r) {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= r)
        temp[k++] = arr[j++];

    for (i = l; i <= r; i++)
        arr[i] = temp[i];
}

void mergeSort(int l, int r, int depth);

void *sortThread(void *arg) {
    struct Data *d = (struct Data *)arg;

    mergeSort(d->left, d->right, d->depth);

    return NULL;
}

void mergeSort(int l, int r, int depth) {
    if (l >= r)
        return;

    int mid = (l + r) / 2;

    if (depth < MAX_DEPTH) {
        pthread_t t1, t2;

        struct Data d1 = {l, mid, depth + 1};
        struct Data d2 = {mid + 1, r, depth + 1};

        pthread_mutex_lock(&lock);
        threadCount += 2;
        pthread_mutex_unlock(&lock);

        pthread_create(&t1, NULL, sortThread, &d1);
        pthread_create(&t2, NULL, sortThread, &d2);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    else {
        mergeSort(l, mid, depth + 1);
        mergeSort(mid + 1, r, depth + 1);
    }

    merge(l, mid, r);
}

int main() {
    struct timeval start, end;

    pthread_mutex_init(&lock, NULL);

    int values[SIZE] = {
        12, 5, 8, 2, 15, 1, 9, 4,
        11, 3, 14, 6, 10, 7, 16, 13
    };

    for (int i = 0; i < SIZE; i++)
        arr[i] = values[i];

    gettimeofday(&start, NULL);

    mergeSort(0, SIZE - 1, 0);

    gettimeofday(&end, NULL);

    double time = (end.tv_sec - start.tv_sec) * 1000000.0 +
                  (end.tv_usec - start.tv_usec);

    printf("Sorted array:\n");

    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);

    printf("\n");

    printf("Number of threads created: %d\n", threadCount);
    printf("Execution time: %.2f microseconds\n", time);

    pthread_mutex_destroy(&lock);

    return 0;
}

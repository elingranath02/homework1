#include <fcntl.h>  // För O_CREAT
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
#define maxSleep 5
#define maxSleepBath 1

sem_t *bathroomLock, *waitroomLock;
int countInBathroom = 0;
int countInQueue = 0;
int genderInBathroom = -1;
int lock;
int lock2;
int inBathroom = 0;

int randomTime(int maxTime) {
    int r = rand() % maxTime;

    return r;
}

void enterBathroom(int id) {
    pthread_mutex_lock(&lock);
    if (countInBathroom == 0) {
        genderInBathroom = id % 2;
        printf("Gender in bathroom: %d\n", genderInBathroom);
        sem_wait(bathroomLock);
    } else {
        if (id % 2 != genderInBathroom || countInQueue != 0) {
            countInQueue++;
            printf("Person %d väntar på att använda badrummet.\n", id);
            sem_wait(bathroomLock);
            countInQueue--;
        }
    }

    printf("Person %d använder badrummet.\n", id);
    countInBathroom++;
    sleep(1);
    pthread_mutex_unlock(&lock);

    printf("Person %d är klar.\n", id);
    countInBathroom--;
    if (countInBathroom == 0) {
        sem_post(bathroomLock);
    }
}

void *start(void *arg) {
    int id = *(int *)arg;

    sleep(1);
    enterBathroom(id);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    bathroomLock = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    waitroomLock = sem_open("/waitroomSem", O_CREAT, 0644, 1);
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, start, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);

    return 0;
}

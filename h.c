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
int lock;
int countInBathroom = 0;
int countInQueue = 0;
int genderInBathroom = -1;

int randomTime(int maxTime) { return rand() % maxTime; }

void enterBathroom(int id) {
    sem_wait(waitroomLock);
    pthread_mutex_lock(&lock);

    if (countInBathroom == 0) {
        genderInBathroom = id % 2;
        printf("Gender in bathroom: %d\n", genderInBathroom);
        sem_post(waitroomLock);
        sem_wait(bathroomLock);
    } else {
        sem_post(waitroomLock);
        if (id % 2 != genderInBathroom || countInQueue != 0) {
            countInQueue++;
            printf("Person %d väntar på att använda badrummet.\n", id);
            sem_wait(bathroomLock);
            countInQueue--;
        }
    }

    countInBathroom++;
    pthread_mutex_unlock(&lock);

    printf("Person %d använder badrummet.\n", id);
    sleep(1);

    pthread_mutex_lock(&lock);
    countInBathroom--;
    if (countInBathroom == 0) {
        sem_post(bathroomLock);
    }
    pthread_mutex_unlock(&lock);

    printf("Person %d är klar.\n", id);
}

void *start(void *arg) {
    int id = *(int *)arg;
    free(arg);
    sleep(1);
    enterBathroom(id);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    bathroomLock = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    waitroomLock = sem_open("/waitroomSem", O_CREAT, 0644, 1);
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));  // Undviker race condition
        *arg = i;
        pthread_create(&threads[i], NULL, start, arg);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    pthread_mutex_destroy(&lock);

    return 0;
}

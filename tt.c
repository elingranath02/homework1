
#include <fcntl.h>  // För O_CREAT
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define maxSleep 3
#define maxSleepBath 1

sem_t *bathroomLock, *waitroomLock, *womenLock, *menLock;
int countInBathroom = 0;
int countInQueue = 0;
int genderInBathroom = -1;
pthread_mutex_t lock, lock2;
int firstTime = 0;
// int *lock2;
int inBathroom = 0;

int randomTime(int maxTime) {
    int r = rand() % maxTime;

    return r;
}

void enterBathroom(int id) {
    pthread_mutex_lock(&lock);
    if (countInBathroom == 0) {
        countInBathroom++;
        pthread_mutex_lock(&lock2);
        genderInBathroom = id % 2;
        pthread_mutex_unlock(&lock2);
        printf("Person %d in bathroom\n", id);
        printf("Number of people in bathrooom: %d\n", countInBathroom);
        pthread_mutex_lock(&lock2);

        if (id % 2 == 0) {
            if (firstTime != 0) {
                sem_wait(menLock);
            } else {
                menLock = sem_open("/menSem", O_CREAT, 0644, 0);
            }
            printf("%s\n", "Closed for men");
            pthread_mutex_unlock(&lock2);
            pthread_mutex_unlock(&lock);
        } else {
            if (firstTime != 0) {
                sem_wait(womenLock);
            } else {
                womenLock = sem_open("/womenSem", O_CREAT, 0644, 0);
            }

            printf("%s\n", "Closed for women");
            pthread_mutex_unlock(&lock2);
            pthread_mutex_unlock(&lock);
        }
        firstTime++;

        sleep(1);
        printf("Person %d is done\n", id);
        countInBathroom--;

    } else if ((countInBathroom != 0) && (id % 2 == genderInBathroom) &&
               (countInQueue == 0)) {
        countInBathroom++;
        printf("Person %d in bathroom \n", id);

        pthread_mutex_lock(&lock2);
        genderInBathroom = id % 2;
        pthread_mutex_unlock(&lock2);
        printf("Number of people in bathroom: %d\n", countInBathroom);
        sleep(1);
        printf("Person %d is done \n", id);
        countInBathroom--;

    } else if ((countInBathroom != 0) && (id % 2 != genderInBathroom) &&
               (countInQueue == 0)) {
        printf("Person %d in queue : \n", id);
        countInQueue++;

        printf("Number of people in queue: %d\n", countInQueue);

        usleep(1);

        if (countInBathroom == 0) {
            if (id % 2 == 0) {
                sem_wait(womenLock);
                sem_wait(womenLock);
                printf("Person %d in bathroom \n", id);
                countInBathroom++;
                countInQueue--;
                printf("Number of people in bathroom: %d\n", countInBathroom);
                printf("Number of people in queue: %d\n", countInQueue);
                sleep(1);
                printf("Person %d is done \n", id);
                countInBathroom--;

            } else {
                sem_wait(menLock);
                sem_wait(menLock);
                printf("Person %d in bathroom \n", id);
                countInBathroom++;
                countInQueue--;
                printf("Number of people in bathroom: %d\n", countInBathroom);
                printf("Number of people in queue: %d\n", countInQueue);
                sleep(1);
                printf("Person %d is done \n", id);
                countInBathroom--;
            }

        } else {
            if (id % 2 == 0) {
                sem_wait(womenLock);
                // sem_post(womenLock);
                printf("Person %d in bathrooom\n", id);
                countInBathroom++;
                countInQueue--;
                sleep(1);
                printf("Person %d is done\n", id);
                countInBathroom--;

            } else {
                sem_wait(menLock);
                // sem_post(menLock);
                printf("Person %d in bathroom\n", id);
                countInBathroom++;
                countInQueue--;
                printf("Number of people in bathroom: %d\n", countInBathroom);
                printf("Number of people in queue: %d\n", countInQueue);

                sleep(1);
                printf("Person %d is done\n", id);
                countInBathroom--;
            }
        }
    }

    // countInBathroom--;
    pthread_mutex_unlock(&lock2);

    printf("Number of people in bathroom: %d\n", countInBathroom);
    printf("Number of people in queue: %d\n", countInQueue);

    // pthread_mutex_lock(&lock2);
    if (countInBathroom == 0 && countInQueue == 0) {
        sem_post(menLock);
        sem_post(womenLock);
        pthread_mutex_unlock(&lock);
    }

    if (countInBathroom == 0 && countInQueue != 0) {
        if (id % 2 == 0) {
            sem_post(menLock);
        } else {
            sem_post(womenLock);
        }
        // pthread_mutex_unlock(&lock2);
    }
}

void *start(void *arg) {
    int id = *(int *)arg;
    free(arg);  // Undvik minnesläckor
    enterBathroom(id);
    return NULL;
}

int main() {
    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);

    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    randomTime(maxSleep);
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);
    bathroomLock = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    waitroomLock = sem_open("/waitroomSem", O_CREAT, 0644, 1);
    // womenLock = sem_open("/womenSem", O_CREAT, 0644, 1);
    // menLock = sem_open("/menSem", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        int *tid = malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, start, tid);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock2);

    return 0;
}
/*} else if ((countInBathroom != 0) && (id % 2 == genderInBathroom) &&
               (countInQueue == 0)) {
        countInBathroom++;
        printf("%s", "YOl");
    } else if ((countInBathroom != 0) && (id % 2 != genderInBathroom) &&
               (countInQueue == 0)) {
        sem_close(bathroomLock);
        sem_wait(bathroomLock);
        printf("%s\n", "YOO");
        countInQueue++;
        genderInBathroom = id % 2;
        printf("gender in bath: %d\n", genderInBathroom);
    } */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define maleThreads 25
#define womenThreads 25
#define maxNrOfThreads 50
#define maxSleep 5000
#define maxSleepBathroom 1000
#define SHARED 1
sem_t *bathroomLock;  // 1 om den är ledig, 0 om inte ledig
sem_t *waitroomLock;  // 1 om den är ledig, 0 om inte ledig

const char sem_name_bathroomLock[] = "./bathroomLock";
const char sem_name_waitroomLock[] = "./waitroomLock";

pthread_t threads[maxNrOfThreads];

int numInBathroom = 0;
int isInBathroom = NULL;
int head = 0;
int tail = 0;
int waitroomEmtpy = 0;

int randomTime(int maxTime) {
    int r = rand() % maxTime;

    return r;
}

void setGenderBathroom(int id) {
    int inBathroom = id % 2;
    bathroomLock = 1;
    return;
}

void enterBathroom(int id) {
    printf("Semaphore: %d", waitroomLock);
    while (1) {
        int bathroom, waitroom;
        sem_getvalue(&bathroomLock, &bathroom);
        sem_getvalue(&waitroomLock, &waitroom);

        if (bathroom && waitroom) {
            printf("This is thread nr: %d", id);
            setGenderBathroom(id);
            return;
        }

        if (bathroom == 0 && waitroom == 1) {
            if (id % 2 == isInBathroom) {
                return;
            } else {
                sem_wait(&waitroomLock);
                sem_wait(&bathroomLock);
                setGenderBathroom(id);
                sem_post(&waitroomLock);
                return;
            }
        }
    }
}

void leaveBathroom() {
    numInBathroom--;
    if (numInBathroom == 0) {
        isInBathroom = NULL;
    }
}

int womenInQueue = 0;
int menInQueue = 0;

int queue(id) {
    int first;

    if (waitroomLock == 0) {
        first = id % 2;
    }

    if ((id % 2) == 0) {
        womenInQueue++;
    } else {
        menInQueue++;
    }

    while (1) {
        if (numInBathroom == 0) {
            if (first == 0) {
                int tempWomen = womenInQueue;
                womenInQueue = 0;
                return tempWomen;
            } else {
                int tempMen = menInQueue;
                menInQueue = 0;
                return tempMen;
            }
        }
    }
}

void *thread(void *args) {
    randomTime(maxSleep);
    printf("%d\n", maxNrOfThreads);
    printf("%s\n", "hello");
    enterBathroom(args);
    randomTime(maxSleepBathroom);
    leaveBathroom();
}

int main() {
    bathroomLock = sem_open(sem_name_bathroomLock, O_CREAT, 0644, 1);
    waitroomLock = sem_open(sem_name_waitroomLock, O_CREAT, 0644, 1);

    int val = 0;
    sem_getvalue(&bathroomLock, &val);
    printf("First init sem: %d\n", bathroomLock);

    for (int i = 0; i < maxNrOfThreads; i++) {
        pthread_create(&threads[i], NULL, thread, NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);

    return 0;
}

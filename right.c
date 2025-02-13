
#include <fcntl.h>  // För O_CREAT
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define maxSleep 6
#define maxSleepBath 3

sem_t *x, *y, *womenLock, *menLock;
int nrOfWomenInBathroom = 0;
int nrOfMenInBathroom = 0;
int nrOfWomenWaiting = 0;
int nrOfMenWaiting = 0;

const char sem_name_xSem[] = "./xSem";
const char sem_name_ySem[] = "./ySem";
const char sem_name_womenSem[] = "./womenSem";
const char sem_name_menSem[] = "./menSem";

int randomTime(int maxTime) {
    int r = 0;
    while (r == 0) {
        r = rand() % maxTime;
    }

    return r;
}

void enterWoman(int id) {
    nrOfWomenInBathroom++;
    printf("Woman %d enters bathroom\n", id);
    // printf("Nr of women waiting: %d\n", nrOfWomenWaiting);
    sleep(randomTime(maxSleepBath));
    printf("Woman %d leaves bathroom\n", id);
    // nrOfWomenInBathroom--;
}
void enterMan(int id) {
    nrOfMenInBathroom++;
    printf("Man %d enters bathroom\n", id);
    // printf("Nr of men waiting: %d\n", nrOfMenWaiting);
    sleep(randomTime(maxSleepBath));
    printf("Man %d leaves bathroom\n", id);
    // nrOfMenInBathroom--;
}

void women(int id) {
    if ((nrOfMenInBathroom == 0 && nrOfMenWaiting == 0)) {
        // sem_post(menLock);
        sem_wait(menLock);
        // nrOfWomenInBathroom++;
        sem_post(y);
        // printf("Woman joined and set menLock\n");
        enterWoman(id);
    } else {
        printf("Woman %d placed in queue\n", id);
        nrOfWomenWaiting++;
        sem_post(y);
        // printf("Nr of women waiting: %d\n", nrOfWomenWaiting);
        if (nrOfWomenWaiting == 1 && nrOfWomenInBathroom == 0) {
            sem_wait(menLock);
            // printf("Woman set menLock in queue\n");
        }

        sem_wait(womenLock);
        // sem_post(womenLock);

        // printf("Woman joined second\n");
        nrOfWomenWaiting--;
        enterWoman(id);
    }

    // printf("Nr of women in bathroom: %d\n", nrOfWomenInBathroom);
    nrOfWomenInBathroom--;
    if (nrOfWomenInBathroom == 0) {
        sem_post(menLock);
        // printf("Men lock opened by woman\n");
        // printf("Nr of men waiting: %d\n", nrOfMenWaiting);
        for (int i = 0; i < nrOfMenWaiting - 1; i++) {
            // printf("# of men waiting: %d\n", nrOfMenWaiting);
            sem_post(menLock);
            // printf("Men lock opened by woman\n");
        }
    }
}

void men(int id) {
    if ((nrOfWomenInBathroom == 0 && nrOfWomenWaiting == 0)) {
        // sem_post(womenLock);
        sem_wait(womenLock);
        sem_post(y);
        // printf("man joined and set womenLock\n");
        enterMan(id);
    } else {
        printf("Man %d placed in queue\n", id);
        nrOfMenWaiting++;
        sem_post(y);
        // printf("Nr of men waiting: %d\n", nrOfMenWaiting);
        if (nrOfMenWaiting == 1 && nrOfMenInBathroom == 0) {
            sem_wait(womenLock);
            // printf("Man set womenLock in queue\n");
        }
        sem_wait(menLock);
        // sem_post(menLock);

        // printf("man joined second\n");
        nrOfMenWaiting--;
        enterMan(id);
    }
    nrOfMenInBathroom--;
    // printf("Nr of men in bathroom: %d\n", nrOfMenInBathroom);
    if (nrOfMenInBathroom == 0) {
        sem_post(womenLock);
        // printf("Women lock opened by man\n");
        // printf("Nr of women waiting:%d \n", nrOfWomenWaiting);
        for (int i = 0; i < nrOfWomenWaiting - 1; i++) {
            // printf("Women lock opened by man\n");
            sem_post(womenLock);
        }
    }
}

void enterBathroom(int id) {
    // printf("person with id: %d waits at x\n", id);
    // // sem_wait(x);
    // printf("person with id: %d enters first x\n", id);
    sem_wait(y);
    // printf("person with id: %d enters first y\n", id);
    sem_post(x);

    if (id % 2 == 0) {
        women(id);
    } else {
        men(id);
    }
}

void *start(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sleep(randomTime(maxSleep));
        sem_wait(x);
        enterBathroom(id);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    // pthread_detach(threads);

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/xSem");
    sem_unlink("/ySem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    x = sem_open("/xSem", O_CREAT, 0644, 1);
    y = sem_open("/ySem", O_CREAT, 0644, 1);
    womenLock = sem_open("/womenSem", O_CREAT, 0644, 1);
    menLock = sem_open("/menSem", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, start, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/xSem");
    sem_unlink("/ySem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    return 0;
}

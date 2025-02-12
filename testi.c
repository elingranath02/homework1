
#include <fcntl.h>  // För O_CREAT
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
#define maxSleep 5
#define maxSleepBath 3

sem_t *x, *y, *womenLock, *menLock;
int nrOfWomenInBathroom = 0;
int nrOfMenInBathroom = 0;
int nrOfWomenWaiting = 0;
int nrOfMenWaiting = 0;

int randomTime(int maxTime) {
    int r = rand() % maxTime;

    return r;
}

void enterWoman(int id) {
    nrOfWomenWaiting--;
    printf("Woman %d enters bathroom\n", id);
    nrOfWomenInBathroom++;
    sleep(randomTime(maxSleepBath));
    printf("Woman %d leaves bathroom\n", id);
    nrOfWomenInBathroom--;
}
void enterMan(int id) {
    nrOfMenWaiting--;
    printf("Man %d enters bathroom\n", id);
    nrOfMenInBathroom++;
    sleep(randomTime(maxSleepBath));
    printf("Man %d leaves bathroom\n", id);
    nrOfMenInBathroom--;
}

void women(int id) {
    if ((nrOfMenInBathroom == 0 && nrOfWomenInBathroom == 0) &&
        (menLock && womenLock)) {
        printf("%s\n", "hellooooo");
        sem_post(y);
        // sem_post(menLock);
        sem_wait(menLock);
        printf("woman joined first and set manLock\n");
        enterWoman(id);
    } else {
        nrOfWomenWaiting++;
        sem_post(y);
        printf("Woman %d placed in queue\n", id);
        sem_wait(womenLock);
        sem_post(womenLock);
        if (nrOfWomenInBathroom == 0) {
            sem_wait(menLock);
            printf("set menLock\n");
        }
        printf("woman joined second\n");
        enterWoman(id);
    }
    if (nrOfWomenInBathroom == 0 && nrOfWomenWaiting == 0) {
        printf("Women open manLock\n");
        for (int i = 0; i <= nrOfMenWaiting + 1; i++) {
            sem_post(menLock);
        }
    }
}

void men(int id) {
    if ((nrOfMenInBathroom == 0 && nrOfWomenInBathroom == 0) &&
        (menLock && womenLock)) {
        printf("%s\n", "yooo");
        sem_post(y);
        // sem_post(womenLock);
        sem_wait(womenLock);
        printf("man joined first and set womenLock\n");
        enterMan(id);
    } else {
        nrOfMenWaiting++;
        sem_post(y);
        printf("Man %d placed in queue\n", id);
        sem_wait(menLock);
        sem_post(menLock);
        if (nrOfMenInBathroom == 0) {
            sem_wait(womenLock);
            printf("set womenLock\n");
        }

        printf("man joined second\n");
        enterMan(id);
    }
    if (nrOfMenInBathroom == 0 && nrOfMenWaiting == 0) {
        printf("Man open womanLock\n");
        for (int i = 0; i <= nrOfMenWaiting + 1; i++) {
            printf("Lock opened by man\n");
            sem_post(womenLock);
        }
    }
}

void enterBathroom(int id) {
    sem_wait(x);
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
    sleep(randomTime(maxSleep));
    enterBathroom(id);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    x = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    y = sem_open("/waitroomSem", O_CREAT, 0644, 1);
    womenLock = sem_open("/womenSem", O_CREAT, 0644, 1);
    menLock = sem_open("/menSem", O_CREAT, 0644, 1);

    while (1) {
        for (int i = 0; i < NUM_THREADS; i++) {
            ids[i] = i;
            pthread_create(&threads[i], NULL, start, &ids[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    return 0;
}

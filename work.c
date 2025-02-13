
#include <fcntl.h>  // För O_CREAT
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 1
#define maxSleep 5
#define maxSleepBath 3

sem_t *flora, *y, *womenLock, *menLock;
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
    printf("Nr of women waiting: %d\n", nrOfWomenWaiting);
    nrOfWomenInBathroom++;
    sleep(randomTime(maxSleepBath));
    printf("Woman %d leaves bathroom\n", id);
    nrOfWomenInBathroom--;
}
void enterMan(int id) {
    nrOfMenWaiting--;
    printf("Man %d enters bathroom\n", id);
    printf("Nr of men waiting: %d\n", nrOfMenWaiting);
    nrOfMenInBathroom++;
    sleep(randomTime(maxSleepBath));
    printf("Man %d leaves bathroom\n", id);
    nrOfMenInBathroom--;
}

void women(int id) {
    if ((menLock && womenLock) == 0) {
        printf("Both locks closed");
    }
    if ((nrOfMenInBathroom == 0 && nrOfWomenInBathroom == 0) &&
        (menLock && womenLock)) {
        printf("%s\n", "hellooooo");

        // sem_post(menLock);
        if ((menLock && womenLock) == 0) {
            printf("Both locks closed \n");
        }
        sem_wait(menLock);
        sem_post(y);
        printf("woman joined first and set manLock\n");
        nrOfWomenWaiting++;
        enterWoman(id);
    } else {
        printf("Woman %d placed in queue\n", id);
        nrOfWomenWaiting++;
        printf("Nr of women waiting: %d\n", nrOfWomenWaiting);
        if ((menLock && womenLock) == 0) {
            printf("Both locks closed 2\n");
        }
        sem_wait(womenLock);
        sem_post(womenLock);
        sem_post(y);
        if (nrOfWomenInBathroom == 0) {
            sem_wait(menLock);
            printf("set menLock\n");
        }
        printf("woman joined second\n");
        enterWoman(id);
    }
    if (nrOfWomenInBathroom == 0 && nrOfWomenWaiting == 0) {
        printf("Women open manLock\n");
        for (int i = 0; i <= nrOfMenWaiting; i++) {
            sem_post(menLock);
        }
    }
}

void men(int id) {
    if ((nrOfMenInBathroom == 0 && nrOfWomenInBathroom == 0) &&
        (menLock && womenLock)) {
        printf("%s\n", "yooo");

        // sem_post(womenLock);
        if ((menLock && womenLock) == 0) {
            printf("Both locks closed 3\n");
        }
        sem_wait(womenLock);
        sem_post(y);
        printf("man joined first and set womenLock\n");
        nrOfMenWaiting++;
        enterMan(id);
    } else {
        printf("Man %d placed in queue\n", id);
        nrOfMenWaiting++;
        printf("Nr of men waiting: %d\n", nrOfMenWaiting);
        if ((menLock && womenLock) == 0) {
            printf("Both locks closed 4\n");
        }
        sem_wait(menLock);
        sem_post(menLock);
        sem_post(y);
        if (nrOfMenInBathroom == 0) {
            sem_wait(womenLock);
            printf("set womenLock\n");
        }

        printf("man joined second\n");
        enterMan(id);
    }
    if (nrOfMenInBathroom == 0 && nrOfMenWaiting == 0) {
        printf("Man open womanLock\n");
        for (int i = 0; i <= nrOfMenWaiting; i++) {
            printf("Lock opened by man\n");
            sem_post(womenLock);
        }
    }
}

void enterBathroom(int id) {
    int a;
    sem_getvalue(flora, &a);
    printf("Lockvalue: %d\n", flora);
    printf("person with id: %d waits at flora\n", id);
    sem_wait(flora);
    sem_getvalue(flora, &a);
    printf("Lockvalue after : %d\n", flora);
    sem_wait(flora);
    sem_getvalue(flora, &a);
    printf("Lockvalue after : %d\n", flora);

    printf("person with id: %d enters first flora\n", id);
    sem_wait(y);
    printf("person with id: %d enters first y and opens flora\n", id);
    sem_post(flora);
    /*
        if (id % 2 == 0) {
            women(id);
        } else {
            men(id);
        }*/

    sleep(randomTime(maxSleep));
    printf("person with id: %d opens y\n", id);
    sem_post(y);
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

    flora = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    y = sem_open("/waitroomSem", O_CREAT, 0644, 1);
    womenLock = sem_open("/womenSem", O_CREAT, 0644, 1);
    menLock = sem_open("/menSem", O_CREAT, 0644, 1);

    sem_close(flora);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");
    sem_destroy(flora);
    sem_destroy(y);

    flora = sem_open("/bathroomSem", O_CREAT, 0644, 1);

    int a = 0;
    sem_getvalue(flora, &a);
    printf("Lockvalue: %d\n", flora);
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

    sem_close(flora);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    return 0;
}

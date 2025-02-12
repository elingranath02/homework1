#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
#define MAX_BATHROOM_TIME 2
#define MAX_WAIT_TIME 4

sem_t *bathroomLock, *waitroomLock;

int menInBathroom = 0;
int womenInBathroom = 0;
int menWaiting = 0;
int womenWaiting = 0;

int randomTime(int maxTime) {
    int r = rand() % maxTime;
    return r;
}

void inBathroom(int id) {
    if (id % 2 == 0) {
        womenInBathroom++;
        printf("Woman in bathroom: %d\n", id);
        sleep(randomTime(MAX_BATHROOM_TIME));
        womenInBathroom--;
        printf("Woman left bathroom: %d in bathroom: %d\n", id,
               womenInBathroom);
    } else {
        menInBathroom++;
        printf("Man in bathroom: %d\n", id);
        sleep(randomTime(MAX_BATHROOM_TIME));
        menInBathroom--;
        printf("Man left bathroom: %d in bathroom: %d\n", id, menInBathroom);
    }
    sem_post(bathroomLock);
    return;
}

void waitLineMen(int id) {
    menWaiting++;
    printf("Man waiting: %d\n", id);
    while (womenInBathroom > 0 || womenWaiting > 0) {
        sem_post(waitroomLock);
        sem_wait(waitroomLock);
    }
    menWaiting--;
    sem_post(waitroomLock);
    inBathroom(id);
    return;
}

void waitLineWomen(int id) {
    womenWaiting++;
    printf("Woman waiting: %d\n", id);
    while (menInBathroom > 0 || menWaiting > 0) {
        sem_post(waitroomLock);
        sem_wait(waitroomLock);
    }
    womenWaiting--;
    sem_post(waitroomLock);
    inBathroom(id);
    return;
}

void man(int id) {
    sem_wait(bathroomLock);
    if (womenInBathroom > 0 || womenWaiting > 0) {
        sem_post(bathroomLock);
        waitLineMen(id);
        return;
    } else {
        sem_post(waitroomLock);
        inBathroom(id);
        return;
    }
}

void woman(int id) {
    sem_wait(bathroomLock);
    if (menInBathroom > 0 || menWaiting > 0) {
        sem_post(bathroomLock);
        waitLineWomen(id);
        return;
    } else {
        sem_post(waitroomLock);
        inBathroom(id);
        return;
    }
}

void *start(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 10; i++) {
        sleep(randomTime(MAX_WAIT_TIME));
        sem_wait(waitroomLock);
        if (id % 2 == 0) {
            woman(id);
        } else {
            man(id);
        }
    }
}

int main() {
    sem_unlink("/bathroom");
    sem_unlink("/waitroom");

    srand(time(NULL));
    pthread_t people[N];
    int ids[N];

    bathroomLock = sem_open("/bathroom", O_CREAT | O_EXCL, 0644, 1);
    waitroomLock = sem_open("/waitroom", O_CREAT | O_EXCL, 0644, 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i + 1;
        if (i % 2 == 0) {
            pthread_create(&people[i], NULL, start, &ids[i]);
        } else {
            pthread_create(&people[i], NULL, start, &ids[i]);
        }
    }
    for (int i = 0; i < N; i++) {
        pthread_join(people[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroom");
    sem_unlink("/waitroom");

    return 0;
}
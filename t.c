#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define USE_TIME 2
#define WORK_TIME 5

sem_t *bathroomLock, *waitroomLock;
int genderCount = 0;  // Antal personer i badrummet
int gender = -1;      // -1 = tomt, 0 = kvinnor, 1 = män

void use_bathroom(int id) {
    printf("Person %d (Gender: %d) is using the bathroom\n", id, id % 2);
    sleep(rand() % USE_TIME + 1);
}

void enter_bathroom(int id) {
    int myGender = id % 2;  // 0 = kvinna, 1 = man

    sem_wait(waitroomLock);  // Vänta på rättvis tillgång
    sem_wait(bathroomLock);  // Skydda könshanteringen

    if (genderCount == 0) {
        gender = myGender;  // Första personen bestämmer könet
    }

    while (gender != myGender) {
        sem_post(bathroomLock);  // Släpp låset om könet är fel
        usleep(1000);            // Undvik busy-wait
        sem_wait(bathroomLock);
    }

    genderCount++;  // Räkna personer i badrummet
    printf("Person %d (Gender: %d) ENTERS the bathroom. Count: %d\n", id,
           myGender, genderCount);

    sem_post(bathroomLock);  // Släpp låset för andra trådar
    sem_post(waitroomLock);  // Tillåt nästa person att försöka gå in

    use_bathroom(id);
}

void leave_bathroom(int id) {
    sem_wait(bathroomLock);
    genderCount--;

    printf("Person %d (Gender: %d) LEAVES the bathroom. Remaining: %d\n", id,
           id % 2, genderCount);

    if (genderCount == 0) {
        gender = -1;  // Om ingen är kvar, badrummet är tomt
    }

    sem_post(bathroomLock);
}

void* person(void* arg) {
    int id = *(int*)arg;
    free(arg);

    while (1) {
        enter_bathroom(id);
        leave_bathroom(id);

        int workTime = rand() % WORK_TIME + 1;
        printf("Person %d (Gender: %d) is working for %d seconds\n", id, id % 2,
               workTime);
        sleep(workTime);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];

    bathroomLock = sem_open("/bathroomSem", O_CREAT, 0644, 1);
    waitroomLock = sem_open("/waitroomSem", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, person, id);
        usleep(100000);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");

    return 0;
}

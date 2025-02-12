#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_MEN 5
#define NUM_WOMEN 5

sem_t *bathroomLock, *waitroomLock;
int men_in_bathroom = 0;
int women_in_bathroom = 0;
int men_waiting = 0;
int women_waiting = 0;

void *man(void *id) {
    int tid = *(int *)id;
    while (1) {
        sleep(1);  // Reduced sleep time for debugging

        sem_wait(bathroomLock);  // Lock to modify shared variables
        men_waiting++;
        printf("Man %d is waiting. Men waiting: %d, Women waiting: %d\n", tid,
               men_waiting, women_waiting);
        sem_post(bathroomLock);

        sem_wait(waitroomLock);  // Ensure mutual exclusion on waitroomLock

        sem_wait(bathroomLock);  // Lock to check bathroom conditions
        men_waiting--;
        while (women_in_bathroom > 0) {  // Wait until no women are inside
            sem_post(bathroomLock);      // Release lock so others can proceed
            sleep(1);                    // Wait for a bit before trying again
            sem_wait(bathroomLock);      // Reacquire lock
        }
        men_in_bathroom++;
        printf("Man %d enters the bathroom. Men inside: %d\n", tid,
               men_in_bathroom);
        sem_post(bathroomLock);
        sem_post(waitroomLock);  // Allow others to access the waiting room

        sleep(1);  // Simulate the time spent in the bathroom

        sem_wait(bathroomLock);  // Lock to modify shared variables
        men_in_bathroom--;
        printf("Man %d leaves the bathroom. Men inside: %d\n", tid,
               men_in_bathroom);
        sem_post(bathroomLock);
    }
}

void *woman(void *id) {
    int tid = *(int *)id;
    while (1) {
        sleep(1);  // Reduced sleep time for debugging

        sem_wait(bathroomLock);  // Lock to modify shared variables
        women_waiting++;
        printf("Woman %d is waiting. Men waiting: %d, Women waiting: %d\n", tid,
               men_waiting, women_waiting);
        sem_post(bathroomLock);

        sem_wait(waitroomLock);  // Ensure mutual exclusion on waitroomLock

        sem_wait(bathroomLock);  // Lock to check bathroom conditions
        women_waiting--;
        while (men_in_bathroom > 0) {  // Wait until no men are inside
            sem_post(bathroomLock);    // Release lock so others can proceed
            sleep(1);                  // Wait for a bit before trying again
            sem_wait(bathroomLock);    // Reacquire lock
        }
        women_in_bathroom++;
        printf("Woman %d enters the bathroom. Women inside: %d\n", tid,
               women_in_bathroom);
        sem_post(bathroomLock);
        sem_post(waitroomLock);  // Allow others to access the waiting room

        sleep(1);  // Simulate the time spent in the bathroom

        sem_wait(bathroomLock);  // Lock to modify shared variables
        women_in_bathroom--;
        printf("Woman %d leaves the bathroom. Women inside: %d\n", tid,
               women_in_bathroom);
        sem_post(bathroomLock);
    }
}

int main() {
    srand(time(NULL));

    bathroomLock = sem_open("/bathroomSem", O_CREAT | O_EXCL, 0644, 1);
    waitroomLock = sem_open("/waitroomSem", O_CREAT | O_EXCL, 0644, 1);

    pthread_t men_threads[NUM_MEN], women_threads[NUM_WOMEN];
    int ids[NUM_MEN > NUM_WOMEN ? NUM_MEN : NUM_WOMEN];

    for (int i = 0; i < NUM_MEN; i++) {
        ids[i] = i + 1;
        pthread_create(&men_threads[i], NULL, man, &ids[i]);
    }
    for (int i = 0; i < NUM_WOMEN; i++) {
        ids[i] = i + 1;
        pthread_create(&women_threads[i], NULL, woman, &ids[i]);
    }

    for (int i = 0; i < NUM_MEN; i++) {
        pthread_join(men_threads[i], NULL);
    }
    for (int i = 0; i < NUM_WOMEN; i++) {
        pthread_join(women_threads[i], NULL);
    }

    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroomSem");
    sem_unlink("/waitroomSem");

    return 0;
}

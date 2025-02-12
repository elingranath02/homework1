#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PEOPLE 10        // Antal personer i simuleringen
#define MAX_BATHROOM_TIME 2  // Max tid i badrummet
#define MAX_WAIT_TIME 5      // Max tid att vänta mellan besök

sem_t *bathroomLock;  // Ser till att bara ett kön åt gången är inne
sem_t *queueLock;     // Hanterar rättvis tillgång

int menCount = 0, womenCount = 0;
pthread_mutex_t countLock = PTHREAD_MUTEX_INITIALIZER;  // Skyddar räknare

void *man(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % MAX_WAIT_TIME + 1);  // Simulera arbete

        sem_wait(queueLock);  // Säkerställ rättvis turordning
        pthread_mutex_lock(&countLock);
        if (menCount == 0) {
            sem_wait(bathroomLock);  // Första mannen blockerar badrummet
        }
        menCount++;
        pthread_mutex_unlock(&countLock);
        sem_post(queueLock);  // Släpp kön

        printf("Man %d enters the bathroom. (Men: %d, Women: %d)\n", id,
               menCount, womenCount);
        sleep(rand() % MAX_BATHROOM_TIME + 1);  // Simulera badrumsbesök

        pthread_mutex_lock(&countLock);
        menCount--;
        printf("Man %d leaves the bathroom. (Men: %d, Women: %d)\n", id,
               menCount, womenCount);
        if (menCount == 0) {
            sem_post(bathroomLock);  // Sista mannen släpper badrummet
        }
        pthread_mutex_unlock(&countLock);
    }
    return NULL;
}

void *woman(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(rand() % MAX_WAIT_TIME + 1);  // Simulera arbete

        sem_wait(queueLock);  // Säkerställ rättvis turordning
        pthread_mutex_lock(&countLock);
        if (womenCount == 0) {
            sem_wait(bathroomLock);  // Första kvinnan blockerar badrummet
        }
        womenCount++;
        pthread_mutex_unlock(&countLock);
        sem_post(queueLock);  // Släpp kön

        printf("Woman %d enters the bathroom. (Men: %d, Women: %d)\n", id,
               menCount, womenCount);
        sleep(rand() % MAX_BATHROOM_TIME + 1);  // Simulera badrumsbesök

        pthread_mutex_lock(&countLock);
        womenCount--;
        printf("Woman %d leaves the bathroom. (Men: %d, Women: %d)\n", id,
               menCount, womenCount);
        if (womenCount == 0) {
            sem_post(bathroomLock);  // Sista kvinnan släpper badrummet
        }
        pthread_mutex_unlock(&countLock);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t people[NUM_PEOPLE];
    int ids[NUM_PEOPLE];

    // Initiera semaforer på macOS-vänligt sätt
    bathroomLock = sem_open("/bathroom", O_CREAT | O_EXCL, 0644, 1);
    queueLock = sem_open("/queue", O_CREAT | O_EXCL, 0644, 1);

    // Skapa trådar för män och kvinnor
    for (int i = 0; i < NUM_PEOPLE; i++) {
        ids[i] = i + 1;
        if (i % 2 == 0) {
            pthread_create(&people[i], NULL, man, &ids[i]);
        } else {
            pthread_create(&people[i], NULL, woman, &ids[i]);
        }
    }

    // Vänta på att trådarna ska köra (de kör oändligt)
    for (int i = 0; i < NUM_PEOPLE; i++) {
        pthread_join(people[i], NULL);
    }

    // Stäng och ta bort semaforer
    sem_close(bathroomLock);
    sem_close(queueLock);
    sem_unlink("/bathroom");
    sem_unlink("/queue");

    return 0;
}

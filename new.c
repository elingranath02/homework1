#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_MEN 5
#define NUM_WOMEN 5

sem_t mutex;
sem_t turnstile;
int men_in_bathroom = 0;
int women_in_bathroom = 0;
int men_waiting = 0;
int women_waiting = 0;

void *man(void *id) {
    int tid = *(int *)id;
    while (1) {
        sleep(rand() % 5 + 1);

        sem_wait(&mutex);
        men_waiting++;
        printf("Man %d is waiting. Men waiting: %d, Women waiting: %d\n", tid,
               men_waiting, women_waiting);
        sem_post(&mutex);

        sem_wait(&turnstile);  // Ensuring fairness
        sem_wait(&mutex);
        men_waiting--;
        if (men_in_bathroom == 0) {
            sem_wait(&turnstile);  // Block women from entering
        }
        men_in_bathroom++;
        printf("Man %d enters the bathroom. Men inside: %d\n", tid,
               men_in_bathroom);
        sem_post(&mutex);
        sem_post(&turnstile);

        sleep(rand() % 3 + 1);

        sem_wait(&mutex);
        men_in_bathroom--;
        printf("Man %d leaves the bathroom. Men inside: %d\n", tid,
               men_in_bathroom);
        if (men_in_bathroom == 0) {
            sem_post(&turnstile);  // Allow women to enter
        }
        sem_post(&mutex);
    }
    return NULL;
}

void *woman(void *id) {
    int tid = *(int *)id;
    while (1) {
        sleep(rand() % 5 + 1);

        sem_wait(&mutex);
        women_waiting++;
        printf("Woman %d is waiting. Men waiting: %d, Women waiting: %d\n", tid,
               men_waiting, women_waiting);
        sem_post(&mutex);

        sem_wait(&turnstile);  // Ensuring fairness
        sem_wait(&mutex);
        women_waiting--;
        if (women_in_bathroom == 0) {
            sem_wait(&turnstile);  // Block men from entering
        }
        women_in_bathroom++;
        printf("Woman %d enters the bathroom. Women inside: %d\n", tid,
               women_in_bathroom);
        sem_post(&mutex);
        sem_post(&turnstile);

        sleep(rand() % 3 + 1);

        sem_wait(&mutex);
        women_in_bathroom--;
        printf("Woman %d leaves the bathroom. Women inside: %d\n", tid,
               women_in_bathroom);
        if (women_in_bathroom == 0) {
            sem_post(&turnstile);  // Allow men to enter
        }
        sem_post(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    sem_init(&mutex, 0, 1);
    sem_init(&turnstile, 0, 1);

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

    sem_destroy(&mutex);
    sem_destroy(&turnstile);
    return 0;
}

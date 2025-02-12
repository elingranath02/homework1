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

int menInBathroom = 0, womenInBathroom = 0, menWaiting = 0, womenWaiting = 0;

int randomTime(int maxTime) { return rand() % maxTime + 1; }

void inBathroom(int id) {
    if (id % 2 == 0) {
        womenInBathroom++;
        printf("Woman in bathroom: %d\n", id);
    } else {
        menInBathroom++;
        printf("Man in bathroom: %d\n", id);
    }

    sleep(randomTime(MAX_BATHROOM_TIME));

    if (id % 2 == 0) {
        womenInBathroom--;
        printf("Woman left bathroom: %d\n", id);
    } else {
        menInBathroom--;
        printf("Man left bathroom: %d\n", id);
    }

    sem_post(bathroomLock);  // Släpp badrumssemafor
}

void waitLineMen(int id) {
    menWaiting++;
    printf("Man waiting: %d\n", id);

    // Vänta på att alla kvinnor lämnar eller väntar
    while (womenInBathroom > 0 || womenWaiting > 0) {
        sem_post(waitroomLock);  // Släpp väntsnemafor
        sem_wait(waitroomLock);  // Vänta på att kvinnorna ska lämna
    }

    menWaiting--;
    sem_post(waitroomLock);  // Släpp väntsnemafor

    inBathroom(id);
}

void waitLineWomen(int id) {
    womenWaiting++;
    printf("Woman waiting: %d\n", id);

    // Vänta på att alla män lämnar eller väntar
    while (menInBathroom > 0 || menWaiting > 0) {
        sem_post(waitroomLock);  // Släpp väntsnemafor
        sem_wait(waitroomLock);  // Vänta på att männen ska lämna
    }

    womenWaiting--;
    sem_post(waitroomLock);  // Släpp väntsnemafor

    inBathroom(id);
}

void man(int id) {
    sem_wait(bathroomLock);  // Vänta på att badrummet är ledigt

    if (womenInBathroom > 0 || womenWaiting > 0) {
        sem_post(bathroomLock);  // Släpp badrumssemafor
        waitLineMen(id);         // Vänta på att kvinnor ska lämna
    } else {
        sem_post(waitroomLock);  // Släpp väntsnemafor
        inBathroom(id);
    }
}

void woman(int id) {
    sem_wait(bathroomLock);  // Vänta på att badrummet är ledigt

    if (menInBathroom > 0 || menWaiting > 0) {
        sem_post(bathroomLock);  // Släpp badrumssemafor
        waitLineWomen(id);       // Vänta på att män ska lämna
    } else {
        sem_post(waitroomLock);  // Släpp väntsnemafor
        inBathroom(id);
    }
}

void *start(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sleep(randomTime(MAX_WAIT_TIME));
        sem_wait(waitroomLock);  // Vänta på att väntsnemafor är tillgänglig

        if (id % 2 == 0) {
            woman(id);  // Om jämn id, kvinna
        } else {
            man(id);  // Om udda id, man
        }
    }
}

int main() {
    sem_close(bathroomLock);
    sem_close(waitroomLock);
    sem_unlink("/bathroom");
    sem_unlink("/waitroom");

    srand(time(NULL));
    pthread_t people[N];
    int ids[N];

    bathroomLock = sem_open("/bathroom", O_CREAT | O_EXCL, 0644, 1);
    waitroomLock = sem_open("/waitroom", O_CREAT | O_EXCL, 0644, 1);

    if (bathroomLock == SEM_FAILED || waitroomLock == SEM_FAILED) {
        perror("Failed to open semaphores");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i + 1;
        pthread_create(&people[i], NULL, start, &ids[i]);
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

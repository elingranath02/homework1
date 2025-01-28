
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*Global variable : accessible to all threads */
int thread_count;
void* Hello(void* rank);
/* Thread function */
int main(int argc, char* argv[]) {
    long thread;

    /* Use long in case of a 64−bit system */
    pthread_t* thread_handles;

    /* Get number of threads from command line */

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
    printf("Hello from the main thread\n");
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    free(thread_handles);
    return 0;
}
/* main */

void* Hello(void* rank) {
    /* Use long in case of 64−bit system */
    long my_rank = (long)rank;
    printf("Hello from thread %ld of %d\n", my_rank, thread_count);
    return NULL;
} /* Hello */

/*int i = row;
    int j = col;
    int k = col;

    while ((i >= 0 && j >= 0 && j <= 7) || (k >= 0 && k >= 0 && k <= 7)) {
        if (boards[thread_num][i][j] == 1 || boards[thread_num][i][k] == 1) {
            return 0;
        }
        i = i - 1;
        j = j + 1;
        k = k - 1;
    }
    int a = row;

    while (a >= 0) {
        if (boards[thread_num][a][col] == 1) {
            return 0;
        }
        a--;
    }

    return 1;*/
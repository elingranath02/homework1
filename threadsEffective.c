#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define NUM_THREADS 8

int solutions = 0;
pthread_t threads[NUM_THREADS];
int boards[NUM_THREADS][8][8] = {{{0}}};
pthread_mutex_t lock;

int diagonal(int n, int row, int col, int thread_num) {
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= 7) {
        if (boards[thread_num][i][j] == 1) {
            return 0;
        }
        i = i - 1;
        j = j + n;
    }

    return 1;
}

int searchUp(int row, int col, int thread_num) {
    int a = row;

    while (a >= 0) {
        if (boards[thread_num][a][col] == 1) {
            return 0;
        }
        a--;
    }
    return 1;
}


int scan(int thread_num, int row, int col) {
    if (
        diagonal(1,row,col, thread_num) == 1 &&
        diagonal(-1,row,col, thread_num) == 1 &&
        searchUp(row, col, thread_num) == 1) {
        return 1;
    } else
        return 0;
}

void print(int thread_num) {
    printf("Solution %d\n", solutions + 1);
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            printf("%d", boards[thread_num][j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
}

void placeFirstQueens(){
    for (int i = 0; i < NUM_THREADS; i++){
        boards[i][0][i] = 1;
    }

}

void recursive(int thread_num, int row) {

    if (row == 8) {
        pthread_mutex_lock(&lock);
        print(thread_num);
        solutions++;
        pthread_mutex_unlock(&lock);
        return;
    }

    for (int col = 0; col < 8; col++) {
        if (scan(thread_num, row, col)) {
            boards[thread_num][row][col] = 1;
            recursive(thread_num, row + 1);
            boards[thread_num][row][col] = 0;
        }
    }
}

void *thread_values (void *args){
    int *argument = (void *)args;
    int thread_num = argument[0];
    int row = argument[1];

    recursive(thread_num, row);

    free(args);
    return NULL;
}

int main() {

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    placeFirstQueens();

    pthread_mutex_init(&lock, NULL);
    
    for (int i = 0; i < NUM_THREADS; i++){
        int *args = malloc(2 * sizeof(int));
        args[0] = i;
        args[1] = 1;
        pthread_create(&threads[i], NULL, thread_values, args);
    }
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    end = clock();

    pthread_mutex_destroy(&lock);

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    int microTime = (cpu_time_used * 1000000);

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    printf("TIME: %0.3d microseconds", microTime);

    
    return 0;
}



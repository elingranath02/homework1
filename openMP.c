#include <omp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 8

int solutions = 0;
pthread_t threads[N];
int boards[N][N][N] = {{{0}}};
omp_lock_t lock;

int diagonal(int n, int row, int col, int thread_num) {
    int i = row;
    int j = col;
    while (i >= 0 && j >= 0 && j <= (N - 1)) {
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
    if (diagonal(1, row, col, thread_num) == 1 &&
        diagonal(-1, row, col, thread_num) == 1 &&
        searchUp(row, col, thread_num) == 1) {
        return 1;
    } else
        return 0;
}

void placeFirstQueens() {
    for (int i = 0; i < N; i++) {
        boards[i][0][i] = 1;
    }
}

double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if (!initialized) {
        gettimeofday(&start, NULL);
        initialized = true;
    }
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void recursive(int thread_num, int row) {
    if (row == N) {
        omp_test_lock(&lock);
        omp_set_lock(&lock);
        solutions++;
        omp_unset_lock(&lock);
        return;
    }

    for (int col = 0; col < N; col++) {
        if (scan(thread_num, row, col)) {
            boards[thread_num][row][col] = 1;
            recursive(thread_num, row + 1);
            boards[thread_num][row][col] = 0;
        }
    }
}

void *thread_values(void *args) {
    recursive(*(int *)args, 1);
    free(args);
    return NULL;
}

int main() {
    double end, start;
    double minTime = 0;

    for (int i = 0; i <= 100; i++) {
        solutions = 0;
        placeFirstQueens();
        omp_init_lock(&lock);
        start = read_timer();

#pragma omp parallel {
        for (int i = 0; i < N; i++) {
            int *args = malloc(sizeof(int));
            *args = i;
            thread_values((void *)args);
            // pthread_create(&threads[i], NULL, thread_values, (void *)args);
        }
    }

    omp_destroy_lock(&lock);
    end = read_timer();

    minTime += (end - start);
}

printf("%s", "NUMBER OF SOLUTIONS: ");
printf("%d\n", solutions);
printf("TIME: %0.3f microseconds", (minTime / 100) * 1000000);

return 0;
}
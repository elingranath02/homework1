#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 8

int solutions = 0;
pthread_t threads[N];
int boards[N][N][N] = {{{0}}};
pthread_mutex_t lock;

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

void print(int thread_num) {
    printf("Solution %d\n", solutions + 1);
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            printf("%d", boards[thread_num][j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
}

void placeFirstQueens() {
    for (int i = 0; i < N; i++) {
        boards[i][0][i] = 1;
    }
}

void recursive(int thread_num, int row) {
    if (row == N) {
        pthread_mutex_lock(&lock);
        print(thread_num);
        solutions++;
        pthread_mutex_unlock(&lock);
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

void *thread_values(args) {
    recursive(args, 1);
    // free(args);
    return NULL;
}

int main() {
    clock_t start, end;
    double cpu_time_used;

    int minTime = __INT_MAX__;

    for (int i = 0; i <= 100; i++) {
        solutions = 0;
        placeFirstQueens();
        pthread_mutex_init(&lock, NULL);
        start = clock();
        int a = 0;
        int b = 1;
        int c = 2;
        int d = 3;
        int e = 4;
        int f = 5;
        int g = 6;
        int h = 7;

        pthread_create(&threads[0], NULL, thread_values, a);
        pthread_create(&threads[1], NULL, thread_values, b);
        pthread_create(&threads[2], NULL, thread_values, c);
        pthread_create(&threads[3], NULL, thread_values, d);
        pthread_create(&threads[4], NULL, thread_values, e);
        pthread_create(&threads[5], NULL, thread_values, f);
        pthread_create(&threads[6], NULL, thread_values, g);
        pthread_create(&threads[7], NULL, thread_values, h);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
        pthread_join(threads[4], NULL);
        pthread_join(threads[5], NULL);
        pthread_join(threads[6], NULL);
        pthread_join(threads[7], NULL);

        pthread_mutex_destroy(&lock);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        int microTime = (cpu_time_used * 1000000);

        if (microTime < minTime) {
            minTime = microTime;
        }
    }

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    printf("TIME: %0.3d microseconds", minTime);

    return 0;
}
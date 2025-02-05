#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 12

int solutions = 0;
double differentTimes[100];

// Diagonal och kolumnkontroller
int diagonal(int board[N][N], int m, int n, int row, int col) {
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= (N - 1)) {
        if (board[i][j] == 1) {
            return 0;
        }
        i = i + m;
        j = j + n;
    }

    return 1;
}

int searchStraight(int board[N][N], int row, int col) {
    int a = row;
    while (a >= 0) {
        if (board[a][col] == 1) {
            return 0;
        }
        a--;
    }
    return 1;
}

int scan(int board[N][N], int row, int col) {
    return diagonal(board, -1, 1, row, col) &&
        diagonal(board, -1, -1, row, col) &&
        searchStraight(board, row, col);
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

void recursive(int board[N][N], int row) {

    if (row == N) {
#pragma omp critical
        {
            solutions++;
        }
        return;
    }

    for (int col = 0; col < N; col++) {
        if (scan(board, row, col)) {
            board[row][col] = 1;
            recursive(board, row + 1);
            board[row][col] = 0;
        }
    }
}

void swap(int i, int j) {
    double temp = differentTimes[i];
    differentTimes[i] = differentTimes[j];
    differentTimes[j] = temp;
}

void insertionSort() {
    for (int i = 0; i < 100; i++) {
        for (int j = i; j > 0 && differentTimes[j] < differentTimes[j - 1]; j--) {
            swap(j, j - 1);
        }
    }
}

int main() {
    double start, end;
    double time = 0;

    omp_set_num_threads(N);

    for (int i = 0; i < 100; i++) {
        solutions = 0;
        start = read_timer();

        #pragma omp parallel
        {
            #pragma omp single
            {
                for (int col = 0; col < N; col++) {
                    #pragma omp task firstprivate(col)
                    {
                        int board[N][N] = {0};
                        board[0][col] = 1;
                        recursive(board, 1);
                    }
                }
            }
        }

        end = read_timer();
        double times = end - start;
        time += times;
        differentTimes[i] = times;
    }

    insertionSort();

    printf("FIRST VAL: %f\n", differentTimes[0] * 1000000);
    printf("MEDIAN TIME: %f\n", differentTimes[49] * 1000000);
    printf("LAST VAL: %f\n", differentTimes[98] * 1000000);

    printf("NUMBER OF SOLUTIONS: %d\n", solutions);
    printf("AVG TIME: %0.3f microseconds\n", (time / 100) * 1000000);

    return 0;
}

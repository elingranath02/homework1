#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 12

int solutions = 0;

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
    if (diagonal(board, -1, 1, row, col) == 1 &&
        diagonal(board, -1, -1, row, col) == 1 &&
        searchStraight(board, row, col) == 1) {
        return 1;
    } else {
        return 0;
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

void recursive(int board[N][N], int row) {
    if (row == N) {
        solutions++;
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

int main() {
    double start, end;
    double time = 0;

    for (int i = 0; i < 100; i++) {
        solutions = 0;
        start = read_timer();
        int board[N][N] = {0};
        recursive(board, 0);
        end = read_timer();


        double times = end - start;
        time += times;
    }

    time = (time / 100) * 1000000;

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    printf("TIME: %0.3f microseconds", time);

    return 0;
}
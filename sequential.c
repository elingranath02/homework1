#include <stdio.h>
#include <sys/time.h>

#define N 8

int solutions = 0;

int diagonal(int board[N][N], int m, int n, int row, int col) {
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= (N-1)) {
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

void print(int board[N][N]) {
    printf("Solution %d\n", solutions + 1);
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
}

void recursive(int board[N][N], int row) {
    if (row == N) {
        print(board);
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
    clock_t start, end;
    double cpu_time_used;
    int time = __INT_MAX__;


    for(int i = 0; i < 10; i++){
        solutions = 0;
        start = clock();
        int board[N][N] = {0};
        recursive(board, 0);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        int microTime = (cpu_time_used * 1000000);

        if(microTime < time){
            time = microTime;
        }
    }

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    printf("TIME: %0.3d microseconds", time);

    return 0;
}
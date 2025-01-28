#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#define MAXTHREADS 3

int solutions = 0;
pthread_t threads[MAXTHREADS];

typedef struct {
    int (*board)[8];
    int n;
    int row;
    int col;
    int *result;
} thread_args;

int diagonal(int (*board)[8], int n, int row, int col) {
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= 7) {
        if (board[i][j] == 1) {
            return 0;
        }
        i = i - 1;
        j = j + n;
    }

    return 1;
}

int searchUp(int (*board)[8], int row, int col) {
    int a = row;

    while (a >= 0) {
        if (board[a][col] == 1) {
            return 0;
        }
        a--;
    }
    return 1;
}

/* int copyBoard(int board[8][8]) {
    int newBoard[8][8];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            newBoard[i][j] = board[i][j];
        }
    }
    return newBoard;
}
*/

void *threadDiagonal(void *args) {
    thread_args *t_args = (thread_args *)args;
    int(*board)[8] = t_args->board;
    int n = t_args->n;
    int row = t_args->row;
    int col = t_args->col;
    *t_args->result = diagonal(board, n, row, col);
    return NULL;

    /*
        int *args = (int *)args;
        int board = copyBoard(arguments);
        int n = *(args + 1);
        int row = *(args + 2);
        int col = *(args + 3);
        int result;

        result = diagonal(board, n, row, col);

        printf("%d", result);

        printf("%s\n", ".....");
        printf("%d\n%d\n%d\n", n, row, col);*/
}

void *threadUp(void *args) {
    thread_args *t_args = (thread_args *)args;
    int(*board)[8] = t_args->board;
    int row = t_args->row;
    int col = t_args->col;
    *t_args->result = searchUp(board, row, col);
    return NULL;

    /*
    int *args = (int *)arguments;
    int board = *args;
    int n = *(args + 1);
    int row = *(args + 2);
    int col = *(args + 3);
    int result = *(args + 4);
    */
}

int scan(int board[8][8], int row, int col) {
    int result1, result2, result3;

    thread_args args1 = {board, 1, row, col, &result1};
    thread_args args2 = {board, -1, row, col, &result2};
    thread_args args3 = {board, 0, row, col, &result3};

    // int diagonal1[5] = {board, 1, row, col, &result1};
    // int diagonal2[5] = {board, -1, row, col, &result2};
    // int up[5] = {board, 0, row, col, &result3};

    pthread_create(&threads[0], NULL, threadDiagonal, &args1);
    pthread_create(&threads[1], NULL, threadDiagonal, &args2);
    pthread_create(&threads[2], NULL, threadUp, &args3);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    if (result1 == 1 && result2 == 1 && result3 == 1) {
        return 1;
    } else
        return 0;
}

void print(int board[8][8]) {
    printf("Solution %d\n", solutions + 1);
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
}

void recursive(int board[8][8], int row) {
    // pthread_t newThread;
    // pthread_create(&newThread, NULL, scan, 1);

    if (row == 8) {
        print(board);
        solutions++;
        return;
    }

    for (int col = 0; col < 8; col++) {
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

    start = clock();
    int board[8][8] = {0};
    recursive(board, 0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    int microTime = (cpu_time_used * 1000000);

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    printf("TIME: %0.3d microseconds", microTime);

    return 0;
}

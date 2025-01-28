#include <stdio.h>
#include <sys/time.h>
int solutions = 0;

int diagonal(int board[8][8], int m, int n, int row, int col) {
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= 7) {
        if (board[i][j] == 1) {
            return 0;
        }
        i = i + m;
        j = j + n;
    }

    return 1;
}

int searchStraight(int board[8][8], int row, int col) {
    int a = row;

    while (a >= 0) {
        if (board[a][col] == 1) {
            return 0;
        }
        a--;
    }
    return 1;
}

int scan(int board[8][8], int row, int col) {
    if (diagonal(board, -1, 1, row, col) == 1 &&
        diagonal(board, -1, -1, row, col) == 1 &&
        searchStraight(board, row, col) == 1) {
        return 1;
    } else {
        return 0;
    }
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

/*int board[8][8] = ((thread_args *)args)->board;
    int n = ((thread_args *)args)->n;
    int row = ((thread_args *)args)->row;
    int col = ((thread_args *)args)->col;
    ((thread_args *)args)->result=diagonal(board,n,row,col)*/

/* typedef struct {
 int (*board)[8];
 int n;
 int row;
 int col;
 int *result;
} thread_args;*/
// int **ptr = malloc(8*sifeof(int*);
// for loop
// int *ptr[i] = malloc(0*sizeof(int))
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define MAXTHREADS

int solutions = 0;

typedef struct
{
    int (*board)[8];
    int n;
    int row;
    int col;
    int *result;
} args;

int diagonal(int board[8][8], int n, int row, int col)
{
    int i = row;
    int j = col;

    while (i >= 0 && j >= 0 && j <= 7)
    {
        if (board[i][j] == 1)
        {
            return 0;
        }
        i = i - 1;
        j = j + n;
    }

    return 1;
}

int searchUp(int board[8][8], int row, int col)
{

    int a = row;

    while (a >= 0)
    {
        if (board[a][col] == 1)
        {
            return 0;
        }
        a--;
    }
    return 1;
}

void *threadDiagonal(void *arguments)
{
    args tArgs = *((args *)arguments);
    *tArgs.result = diagonal(tArgs.board, tArgs.n, tArgs.row, tArgs.col);
    return NULL;
}

void *threadUp(void *arguments)
{
    args tArgs = *((args *)arguments);
    *tArgs.result = searchUp(tArgs.board, tArgs.row, tArgs.col);
    return NULL;
}

int scan(int board[8][8], int row, int col)
{
    int result1, result2, result3;

    args diagonal1 = {board, 1, row, col, &result1};
    args diagonal2 = {board, -1, row, col, &result2};
    args up = {board, 0, row, col, &result3};

    pthread_t threads[3];

    pthread_create(&threads[0], NULL, threadDiagonal, &diagonal1);
    pthread_create(&threads[1], NULL, threadDiagonal, &diagonal2);
    pthread_create(&threads[2], NULL, threadUp, &up);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    if (result1 == 1 && result2 == 1 && result3 == 1)
    {
        return 1;
    }
    else
        return 0;
}

void print(int board[8][8])
{

    printf("Solution %d\n", solutions + 1);
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {

            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
}

void recursive(int board[8][8], int row)
{

    // pthread_t newThread;
    // pthread_create(&newThread, NULL, scan, 1);

    if (row == 8)
    {
        print(board);
        solutions++;
        return;
    }

    for (int col = 0; col < 8; col++)
    {

        if (scan(board, row, col))
        {
            board[row][col] = 1;
            recursive(board, row + 1);
            board[row][col] = 0;
        }
    }
}

int main()
{

    int board[8][8] = {0};
    recursive(board, 0);

    printf("%s", "NUMBER OF SOLUTIONS: ");
    printf("%d\n", solutions);
    return 0;
}

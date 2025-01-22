#include <stdio.h>

void place(int *board)
{

    *board = 1;
    board =board + 64;
    *board = 2;
}

int main()
{

    int board[8][8] = {};

    int i = 0;
    int j = 0;

    place(*board);

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }

    return 0;
}
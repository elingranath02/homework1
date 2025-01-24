#include <stdio.h>

int numberOfQueens = 0;

int x = 0;
int y = 0;

int placeQueens(int board[8][8], int row)
{
    int a = row;
    int b = 0;

    while (b < 8)
    {
        if (board[a][b] == 0)
        {
            board[a][b] = 1;
            numberOfQueens++;
            return b;
        }
        b++;
    }
    return -1;
}

int diagonal(int board[8][8], int m, int n, int mode, int row, int col)
{

    while (row >= 0 && row <= 7 && col >= 0 && col <= 7)
    {
        row = row + m;
        col = col + n;
        if (row >= 0 && row <= 7 && col >= 0 && col <= 7)
        {
            if (mode == 1)
            {
                if(board[row][col] == 0){
                    board[row][col] = 2;
                }else if (board[row][col] == 2) {
                    board[row][col] += 1;
                }
            }else if(mode == 2){
                if(board[row][col] == 2){
                    board[row][col] = 0;
                }else if (board[row][col] > 2) {
                    board[row][col] -= 1;
                }
            }
            else if (board[row][col] == 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

int scanBoard(int board[8][8])
{
    int availableSlots = 0;
    for (int i = 0; i < 8; i++)
    {
        int notAvailableSlots = 0;
        for (int j = 0; j < 8; j++)
        {
            if(board[i][j] == 1){
                break;
            }
            if (board[i][j] == 0)
            {
                availableSlots++;
            }else if(board[i][j] == 2)
            { notAvailableSlots++;
            }
        }
        
        if (notAvailableSlots > 7) {
            printf("Row %d is full (not available): %d slots\n", i, notAvailableSlots);
            return 0;
        }
    }

    if (availableSlots >= (8 - numberOfQueens))
    {
        return 1;
    }
    else
        return 0;
}

void markUnavailableSpots(int board[8][8])
{
}

void queenPosition(int *board)
{
}

int searchStraight(int board[8][8], int row, int col)
{
    int a = row;
    int b = col;

    int temp = a + 1;
    int temp2 = b + 1;

    while (temp != a)
    {

        if (board[temp][b] == 1)
        {
            return 0;
        }

        temp = (temp + 1) % 8;
    }

    while (temp2 != b)
    {

        if (board[a][temp2] == 1)
        {
            return 0;
        }

        temp2 = (temp2 + 1) % 8;
    }

    return 1;
}

void markStraight(int board[8][8], int row, int col)
{

    int a = row;
    int b = col;

    int temp = a + 1;
    int temp2 = b + 1;

    while (temp != a)
    {
        if(board[temp][b] == 0){
            board[temp][b] = 2;
        }else if(board[temp][b] == 2){
            board[temp][b] += 1;
        }
        temp = (temp + 1) % 8;
    }

    while (temp2 != b)
    {
        if(board[a][temp2] == 0){
            board[a][temp2] = 2;
        }else if(board[a][temp2] == 2){
            board[a][temp2] += 1;
        }
        temp2 = (temp2 + 1) % 8;
    }
}

void removeLastQueen(int board[8][8], int row, int col){
        board[row][col] = 2;
        numberOfQueens--;
}

void removeLastQueenAndMarks(int board[8][8], int row, int col){
        board[row][col] = 2;
        numberOfQueens--;
        diagonal(board, 1, 1, 2, row, col);
        diagonal(board, -1, 1, 2, row, col);
        diagonal(board, 1, -1, 2, row, col);
        diagonal(board, -1, -1, 2, row, col);
        markStraight(board, row, col);
}

int mark(int board[8][8], int row, int col)
{
    if (diagonal(board, 1, 1, 0, row, col) == 1 &&
        diagonal(board, -1, 1, 0, row, col) == 1 &&
        diagonal(board, 1, -1, 0, row, col) == 1 &&
        diagonal(board, -1, -1, 0, row, col) == 1 &&
        searchStraight(board, row, col) == 1)
    {
        diagonal(board, 1, 1, 1, row, col);
        diagonal(board, -1, 1, 1, row, col);
        diagonal(board, 1, -1, 1, row, col);
        diagonal(board, -1, -1, 1, row, col);
        markStraight(board, row, col);
        return 1;
    }else
    {
        return 0;
    }
}

void recursive(int board[8][8], int row, int col)
{
    if(numberOfQueens == 8){
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
    return;
    }

    for (int col = 0; col < 8; col++) {

        if(mark(board, row, col)){
            board[row][col] = 1;
            numberOfQueens++;

            recursive(board, row + 1, col);

            board[row][col] = 0;
            numberOfQueens--;
        }
}


int main()
{

    int board[8][8] = {0};

    int i = 0;
    int j = 0;

    recursive(board, x,y);

    //placeQueens(board);
    //mark(board);
    //placeQueens(board);
    //mark(board);
    //placeQueens(board);
    //mark(board);
    //placeQueens(board);
    //mark(board);
    //placeQueens(board);
    //mark(board);

    printf("%s", "NUMBER OF QUEENS: ");
    printf("%d\n", numberOfQueens);

    return 0;
}

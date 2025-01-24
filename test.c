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
                board[row][col] = 2;
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
            temp = (temp + 1) % 8;
        }
        else {
        board[temp][b] +=1;
        temp = (temp + 1) % 8;
        }
    }

    while (temp2 != b)
    {
        if(board[temp][b] ==0){
            board[a][temp2] = 2;
            temp = (temp + 1) % 8;
        }
        else {
        board[a][temp2] +=1;
        temp2 = (temp2 + 1) % 8;
        }
    }
}

void removeLastQueen(int board[8][8], int row, int col){

    if(board[row][col]==0){
        board[row][col] = 2;
        row--;
        numberOfQueens--;
    }
    else {
        board[row][col] += 1;
        row--;
        numberOfQueens--;
    }
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

int recursive(int board[8][8], int row, int col)
{
    if(numberOfQueens == 8){
    return 1;
    }

   placeQueens(board, row);

   if(scanBoard(board) == 1){
        mark(board, row, col);
   }
   if(placeQueens(board, row +1)){
    recursive(board,row,col);
   }
   else {
    removeLastQueen(board, row, col);
    return 0;
   }

   return 1;
}


int main()
{

    int board[8][8] = {0};

    int i = 0;
    int j = 0;

    recursive(board, x,y);

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }

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


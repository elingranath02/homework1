#include <stdio.h>

//int board[8][8];
int numberOfQueens = 0;

//int *queenPlacement;

//int *board = *queenPlacement;

int x=0;
int y=0;




int getQueenPosX(){
    return x;
}

int getQueenPosY(){
    return y;
}

void setQueenPosX(int v){
    x = v;
}

void setQueenPosY(int v){
    y = v;
}

void recursivePlacement(int *board){

   // markUnavailableSpots;

}

void placeQueens(int board[8][8]){
    int a = getQueenPosY();
    int b = getQueenPosX();

    while(a < 8){

        while (b < 8){
            if (board[a][b] != 1 && board[a][b] != 2){
                board[a][b] = 1;
                numberOfQueens++;
                y = a;
                x = b;
                printf("%s\n", "coord: ");
                printf("%d", y);
                printf("%d", x);
                printf("%s\n", "end");
                return;
            }

            if(board[a][b] == 1 || board[a][b] == 2){
                b++;
            }
        }
        a++;
        b = getQueenPosX();
    }
        
}

int diagonal(int board[8][8], int m, int n, int mode){
    int posY = getQueenPosY();
    int posX = getQueenPosX();

    while(posY >= 0 && posY <= 7 && posX >= 0 && posX <= 7){
        posY = posY+m;
        posX = posX+n;
        if(posX || posY == (-1 || 8)){
            return 1;
        }
        if(mode == 1){
            board[posY][posX] = 2;
        }else if (board[posY][posX] == 1){
            //printf("%d\n", posY);
            //printf("%d\n", posX);
            return 0;
        }
    }
    return 1;
}

int scanBoard(int board[8][8]){
    int availableSlots = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] == 0){
                availableSlots++;
            }
        }
    }
    if (availableSlots >= (8 - numberOfQueens)){
        return 1;
    }else return 0;
}



/*x = (x+1)%8;
    while(board[y][x] != 1){
    }*/


void markUnavailableSpots (int board[8][8]){
    
    
}

void queenPosition(int *board){

}

int searchStraight(int board [8][8]) {
    int a = getQueenPosY();
    int b = getQueenPosX();

    int temp = a + 1;
    int temp2 = b +1;

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

void markStraight(int board[8][8]){
    
int a = getQueenPosY();
int b = getQueenPosX();

int temp = a + 1;
int temp2 = b +1;

while (temp != a) {
    board[temp][b] = 2;
    temp = (temp + 1) % 8;
}

while (temp2 != b) {
    board[a][temp2] = 2;
    temp2 = (temp2 + 1) % 8;
}

}

void mark(int board[8][8]){

    int a = diagonal(board,1,1,0);
    int b = diagonal(board,-1,1,0);
    int c = diagonal(board,1,-1,0);
    int d = diagonal(board,-1,-1,0);
    int e = searchStraight(board);

    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    printf("%d\n", d);
    printf("%d\n", e);
    printf("%s\n", "y and x:");
    printf("%d\n", y);
    printf("%d\n", x);
    printf("%s\n", "end");
    



    if(diagonal(board,1,1,0) == 1 &&
        diagonal(board,-1,1,0) == 1 &&
        diagonal(board,1,-1,0) == 1 &&
        diagonal(board,-1,-1,0) == 1 &&
        searchStraight(board) == 1) {
            
        diagonal(board,1,1,1);
        diagonal(board,-1,1,1);
        diagonal(board,1,-1,1);
        diagonal(board,-1,-1,1);
        markStraight(board);
        }
        else {
            board[y][x] = 5;
            numberOfQueens--;
        }
}

void recursive(int board[8][8]){
    
    placeQueens(board);
    
    if(scanBoard(board) == 1){
        mark(board);
    }else {
        printf("%s\n", "ended");
        return;
    }

    recursive(board);

}
    

int main()
{

    int board[8][8] = {0};

    int i = 0;
    int j = 0;

    //recursive(board);

    placeQueens(board);
    mark(board);

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    placeQueens(board);
    mark(board);
    
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    placeQueens(board);
    mark(board);
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    placeQueens(board);
    mark(board);
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    placeQueens(board);
    mark(board);
    //markUnavailableSpots(board);
    //queenPosition(*board);
    //queenPosition(*board);

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", board[j][i]);
            printf("%s", " ");
        }
        printf("%s", "\n");
    }
    printf("%d\n", numberOfQueens);

    return 0;
}

#include <stdio.h>;

//int board[8][8];
int numberOfQueens;

//int *queenPlacement;

//int *board = *queenPlacement;

int x =0;
int y=0;




int recursive(){

    //Always valid state, queen placement is done correctly, can go back to this stage

    //placeQueens();

    //globalQueen++;
    //recursive();
    

}

int recursivePlacement(int *board){

   // markUnavailableSpots;

}

void placeQueens(int board[8][8]){
    int a = y;
    int b = x;

        while (b < 8){
            if (board[a][b] != 1 && board[a][b] != 2){
                board[a][b] = 1;
                y = a;
                x = b;
                return (y,x);
            }

            if(board[a][b] == 1 || board[a][b] == 2){
                b++;
            }
        }
        
}

void diagonal(int board[8][8], int m, int n){

    for(int i = 0; i < 2; i++){
        int posY = getQueenPosY();
        int posX = getQueenPosX();

        while(y || x != -1 || 8){
            posY = posY+m;
            posX = posX+n;
            if(board[posY][posX] == 1){
                return;
            }
            if(i == 1){
                board[posY][posX] = 2;
            }
        }
    }
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
    if (availableSlots > 8 - numberOfQueens){
        return 1;
    }else return 0;
}

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



/*x = (x+1)%8;
    while(board[y][x] != 1){
    }*/


void markUnavailableSpots (int board[8][8]){
    
    
}

void queenPosition(int *board){

}

void straight(int board [8][8]) {
    int a = getQueenPosY();
    int b = getQueenPosX();

    while (board[a][b] =! 1){
    }
    
    
}

int main()
{

    int board[8][8] = {0};

    int i = 0;
    int j = 0;

    placeQueens(board);
    placeQueens(board);
    placeQueens(board);
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

    return 0;
}

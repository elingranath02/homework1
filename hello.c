#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#define N 8  // Storlek på brädet, kan ändras
#define MAX_THREADS N  // En tråd per startkolumn

pthread_mutex_t lock;
int total_solutions = 0;  // Totalt antal lösningar

// En struktur för att hantera varje tråds data
typedef struct {
    int board[N][N]; // Lokalt bräde för varje tråd
    int row;
    int local_solutions;
} ThreadData;

// Kontrollera om en drottning kan placeras på brädet
int is_safe(int board[N][N], int row, int col) {
    // Kontrollera samma kolumn
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return 0;
        }
    }
    // Kontrollera diagonaler
    for (int i = 0; i < row; i++) {
        if (board[i][col - (row - i)] == 1 || board[i][col + (row - i)] == 1) {
            return 0;
        }
    }
    return 1;
}

// Skriver ut brädet när en lösning hittas
void print_board(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Rekursiv funktion för att lösa problemet
void solve(ThreadData *data) {
    int row = data->row;
    if (row == N) {
        // Lösning hittad, skriv ut brädet
        pthread_mutex_lock(&lock);  // Mutex för att säkra att skrivningen inte kolliderar
        print_board(data->board);
        data->local_solutions++;
        pthread_mutex_unlock(&lock);
        return;
    }

    for (int col = 0; col < N; col++) {
        if (is_safe(data->board, row, col)) {
            data->board[row][col] = 1;  // Placera drottningen
            data->row = row + 1;         // Rekursivt placera drottningar på nästa rad
            solve(data);                 // Fortsätt med nästa rad
            data->board[row][col] = 0;  // Ta bort drottningen (backtrack)
        }
    }
}

// Funktion som körs i varje tråd
void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    solve(data);  // Lös problemet för den här tråden

    pthread_mutex_lock(&lock);
    total_solutions += data->local_solutions;  // Lägg till antalet lösningar från denna tråd
    pthread_mutex_unlock(&lock);

    free(data);  // Frigör minne
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&lock, NULL);

    struct timeval start, end;
    gettimeofday(&start, NULL);  // Starta tidtagning

    // Skapa en tråd för varje startkolumn på första raden
    for (int i = 0; i < N; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        // Nollställ brädet
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                data->board[r][c] = 0;

        data->board[0][i] = 1;  // Placera en drottning på första raden i kolumn i
        data->row = 1;  // Börja med andra raden
        data->local_solutions = 0;  // Initiera lokala lösningar för tråden

        pthread_create(&threads[i], NULL, thread_function, (void *)data);  // Skapa en tråd
    }

    // Vänta på att alla trådar ska avslutas
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);  // Vänta på att tråden ska bli klar
    }

    gettimeofday(&end, NULL);  // Sluta tidtagning

    // Tidsberäkning i mikrosekunder
    double elapsed_us = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);

    // Skriv ut den totala tiden och antalet lösningar
    printf("NUMBER OF SOLUTIONS: %d\n", total_solutions);
    printf("TIME: %0.0f microseconds\n", elapsed_us);

    pthread_mutex_destroy(&lock);  // Förstör mutex
    return 0;
}

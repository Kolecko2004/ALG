#include <stdio.h>
#include <stdbool.h> // Pro datový typ bool (dostupné od C99)

#define ROWS 5
#define COLS 5

// Globální pole pro navštívené uzly
bool visited[ROWS][COLS];

void dfs(int r, int c, int matrix[ROWS][COLS]) {
    // 1. Základní kontroly (hranice, hodnota pole, už navštíveno)
    // Upravil jsem podmínku matrix[r][c] == 9, aby to odpovídalo tvé dřívější matici (zdi)
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS || matrix[r][c] == 9 || visited[r][c]) {
        return;
    }

    // 2. Označíme uzel jako navštívený
    visited[r][c] = true;
    printf("Navstivil jsem: [%d, %d] s hodnotou %d\n", r, c, matrix[r][c]);

    // 3. Rekurzivní volání (do hloubky)
    dfs(r + 1, c, matrix); // Dolu
    dfs(r - 1, c, matrix); // Nahoru
    dfs(r, c + 1, matrix); // Doprava
    dfs(r, c - 1, matrix); // Doleva
}

int main() {
    // Inicializace matice
    int matrix[ROWS][COLS] = {
        {1, 3, 1, 1, 1},
        {5, 1, 1, 9, 1},
        {4, 8, 9, 1, 1},
        {1, 1, 1, 1, 9},
        {9, 9, 9, 1, 1}
    };

    // Vynulování pole visited (v C je dobré to udělat explicitně)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }

    printf("Startuji DFS z [0,0]:\n");
    dfs(0, 0, matrix);

    return 0;
}
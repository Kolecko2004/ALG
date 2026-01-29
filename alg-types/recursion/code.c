#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 32

typedef struct {
    int height;
    int width;
    int grid[MAX_SIZE][MAX_SIZE]; // 1 = pole existuje, 0 = chybějící pole
} Board;

/**
 * Načte vstup ze standardního vstupu a uloží jej do struktury Board.
 */
void loadBoard(Board* board) {
    if (scanf("%d %d", &board->height, &board->width) != 2) {
        return;
    }

    for (int i = 0; i < board->height; i++) {
        char rowString[MAX_SIZE + 1];
        scanf("%s", rowString);
        for (int j = 0; j < board->width; j++) {
            // Převod znaku '0'/'1' na integer 0/1
            board->grid[i][j] = rowString[j] - '0';
        }
    }
}

/**
 * Pomocná funkce pro výpis (pro kontrolu správného načtení)
 */
void printBoard(Board* board) {
    printf("Rozmery: %d x %d\n", board->height, board->width);
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            printf("%d", board->grid[i][j]);
        }
        printf("\n");
    }
}

int max_queens = 0;

// Funkce zkontroluje, zda na [r][c] může být dáma
int isSafe(Board* b, int r, int c) {
    // Definice 8 směrů: {dx, dy}
    int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1}, {0, 1}, // kříž
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // diagonály
    };

    for (int i = 0; i < 8; i++) {
        int nr = r + directions[i][0];
        int nc = c + directions[i][1];

        // Šíříme se směrem, dokud nenarazíme na hranu nebo zeď (0)
        while (nr >= 0 && nr < b->height && nc >= 0 && nc < b->width) {
            if (b->grid[nr][nc] == 0)
                break; // Zeď blokuje ohrožení
            if (b->grid[nr][nc] == 2)
                return 0; // Našli jsme jinou dámu!
            nr += directions[i][0];
            nc += directions[i][1];
        }
    }
    return 1;
}

void backtracking(Board* board, int current_count, int start_index) {
    if (current_count > max_queens) {
        max_queens = current_count;
    }

    for (int idy = start_index; idy < board->width * board->height; idy++) {
        int r = idy / board->width;
        int c = idy % board->width;

        if (board->grid[r][c] == 1) {
            if (isSafe(board, r, c)) {
                board->grid[r][c] = 2;
                backtracking(board, current_count + 1, idy + 1);
                board->grid[r][c] = 1;
            }
        }
    }
}

int main() {
    Board board;

    loadBoard(&board);

    // printBoard(&board);

    backtracking(&board, 0, 0);

    fprintf(stdout, "%d\n", max_queens);

    return 0;
}
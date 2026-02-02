#include <stdio.h>
#include <stdbool.h>

#define ROWS 5
#define COLS 5

// Struktura pro souřadnice bodu v matici
typedef struct {
    int r, c;
} Point;

// Jednoduchý zásobník (stack) pro body
typedef struct {
    Point data[ROWS * COLS];
    int top;
} Stack;

void push(Stack *s, int r, int c) {
    s->data[++(s->top)] = (Point){r, c};
}

Point pop(Stack *s) {
    return s->data[(s->top)--];
}

bool is_empty(Stack *s) {
    return s->top == -1;
}

void dfs_iterative(int start_r, int start_c, int matrix[ROWS][COLS]) {
    bool visited[ROWS][COLS] = {false};
    Stack s;
    s.top = -1;

    push(&s, start_r, start_c);

    while (!is_empty(&s)) {
        Point curr = pop(&s);
        int r = curr.r;
        int c = curr.c;

        // Kontrola hranic a překážek
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || matrix[r][c] == 9 || visited[r][c]) {
            continue;
        }

        // Označíme jako navštívené
        visited[r][c] = true;
        printf("Navstivil jsem: [%d, %d] s hodnotou %d\n", r, c, matrix[r][c]);

        // Přidáme sousedy na zásobník (všimni si pořadí, poslední přidaný půjde první ven)
        push(&s, r + 1, c); // Dolů
        push(&s, r - 1, c); // Nahoru
        push(&s, r, c + 1); // Doprava
        push(&s, r, c - 1); // Doleva
    }
}

int main() {
    int matrix[ROWS][COLS] = {
        {1, 3, 1, 1, 1},
        {5, 1, 1, 9, 1},
        {4, 8, 9, 1, 1},
        {1, 1, 1, 1, 9},
        {9, 9, 9, 1, 1}
    };

    printf("Startuji iterativni DFS z [0,0]:\n");
    dfs_iterative(0, 0, matrix);

    return 0;
}
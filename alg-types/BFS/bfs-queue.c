#include <stdio.h>
#include <stdbool.h>

#define ROWS 5
#define COLS 5

typedef struct {
    int r, c;
} Point;

// Fronta pro body
typedef struct {
    Point data[ROWS * COLS];
    int head;
    int tail;
} Queue;

void enqueue(Queue *q, int r, int c) {
    q->data[q->tail++] = (Point){r, c};
}

Point dequeue(Queue *q) {
    return q->data[q->head++];
}

bool is_empty(Queue *q) {
    return q->head == q->tail;
}

void bfs(int start_r, int start_c, int matrix[ROWS][COLS]) {
    bool visited[ROWS][COLS] = {false};
    Queue q = {.head = 0, .tail = 0};

    enqueue(&q, start_r, start_c);
    visited[start_r][start_c] = true;

    while (!is_empty(&q)) {
        Point curr = dequeue(&q);
        printf("Navstivil jsem: [%d, %d]\n", curr.r, curr.c);

        // Definice sousedů (4 směry)
        int dr[] = {1, -1, 0, 0};
        int dc[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            // Kontrola hranic, zdi (9) a zda jsme tam uz nebyli
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && 
                matrix[nr][nc] != 9 && !visited[nr][nc]) {
                
                visited[nr][nc] = true; // Označíme hned při přidání do fronty!
                enqueue(&q, nr, nc);
            }
        }
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
    bfs(0, 0, matrix);
    return 0;
}
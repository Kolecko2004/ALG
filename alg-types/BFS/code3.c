#include <stdio.h>
#include <stdlib.h>

int matrix_size;

typedef struct Node {
    int x, y;
    int value;
    int visited;
    int dist;
} Node;

Node** matrix;

void load_input() {
    fscanf(stdin, "%d\n", &matrix_size);

    matrix = malloc(matrix_size * sizeof(Node*));

    for (int idx = 0; idx < matrix_size; idx++) {
        matrix[idx] = malloc(matrix_size * sizeof(Node));
        for (int idy = 0; idy < matrix_size; idy++) {
            fscanf(stdin, "%d", &matrix[idx][idy].value);
            matrix[idx][idy].x = idx;
            matrix[idx][idy].y = idy;
            matrix[idx][idy].visited = 0;
        }
    }
}

int is_end(int x, int y) {
    if (x == matrix_size - 1 && y == matrix_size - 1) {
        return 1;
    }

    return 0;
}

int queue() {
    Node* queue = malloc(matrix_size * matrix_size * 9 * sizeof(Node));
    int head = 0, tail = 0;


    matrix[0][0].dist = matrix[0][0].value;
    queue[tail++] = matrix[0][0];
    long long smallest_path_length = matrix_size * matrix_size * 9;

    while (head < tail) {
        Node current = queue[head++];
        int moves1[8] = {1, 0, 0, -1, -1, 1, -1, 1};
        int moves2[8] = {0, 1, -1, 0, -1, 1, 1, -1};

        for (int idx = 0; idx < 8; idx++) {
            int nextX = current.x + moves1[idx];
            int nextY = current.y + moves2[idx];

            if (is_end(current.x, current.y) == 1) {
                if (smallest_path_length > current.dist) {
                    smallest_path_length = current.dist;
                }
            }

            if (tail > matrix_size * matrix_size * 9) {
                return smallest_path_length;
            }

            // 1. Kontrola hranic (aby program nespadl)
            if (nextX >= 0 && nextX < matrix_size && nextY >= 0 && nextY < matrix_size) {

                // 2. Kontrola obsahu (teprve teď je to bezpečné)
                if (matrix[nextX][nextY].visited != -1) {
                    matrix[current.x + moves1[idx]][current.y + moves2[idx]].visited = -1;
                    matrix[current.x + moves1[idx]][current.y + moves2[idx]].dist = matrix[current.x + moves1[idx]][current.y + moves2[idx]].value + current.dist;
                    printf("IDX: %d, IDY: %d, DIST: %d\n", current.x + moves1[idx], current.y + moves2[idx], matrix[current.x + moves1[idx]][current.y + moves2[idx]].dist);
                    queue[tail++] = matrix[current.x + moves1[idx]][current.y + moves2[idx]];
                }
            }
        }
    }
    fprintf(stdout, "Here 3\n");
    return smallest_path_length;
}

int main() {
    load_input();

    fprintf(stdout, "%d\n", queue());

    return 0;
}

// 1 3 1 1 1
// 5 1 1 9 1
// 4 8 9 1 1
// 1 1 1 1 9
// 9 9 9 1 1
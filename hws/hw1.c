// Homework 1 - Park

#include <stdio.h>
#include <stdlib.h>

int matrix_size, park_size, center_edge_offset, min_rock_num;
char* load_input();
int find_rocks(const char *matrix, int park_size, int center_edge_offset, int matrix_size, int min_rock_num);

int main() {
    char *matrix = load_input();
    int answer = find_rocks(matrix, park_size, center_edge_offset, matrix_size, min_rock_num);
    fprintf(stdout, "%d\n", answer);
    free(matrix);
    return 0;
}

char* load_input() {
    if (fscanf(stdin, "%d %d %d %d", &matrix_size, &park_size, &center_edge_offset, &min_rock_num) != 4) {
        fprintf(stderr, "Chyba: nepodarilo se nacist 4 povinne parametry.\n");
        exit(100);
    }

    char *main_matrix = malloc(matrix_size * matrix_size * sizeof(char));
    if (!main_matrix) {
        fprintf(stderr, "Chyba: malloc selhal.\n");
        exit(102);
    }

    for (int idx = 0; idx < matrix_size; idx++) {
        for (int idy = 0; idy < matrix_size; idy++) {
            int value;
            if (fscanf(stdin, "%d", &value) != 1) {
                fprintf(stderr, "Chyba: nepodarilo se nacist prvek matice.\n");
                exit(101);
            }
            main_matrix[idx * matrix_size + idy] = (char)value;
        }
    }

    return main_matrix;
}

int prefix_sum(int **ps, int x1, int y1, int x2, int y2) {
    int sum = ps[x2][y2];
    if (x1 > 0) sum -= ps[x1-1][y2];
    if (y1 > 0) sum -= ps[x2][y1-1];
    if (x1 > 0 && y1 > 0) sum += ps[x1-1][y1-1];
    return sum;
}

int find_rocks(const char *matrix, int park_size, int center_edge_offset, int matrix_size, int min_rock_num) {
    int center_block_size = park_size - (2 * center_edge_offset);
    int max_tree_count = 0;

    int **rock_ps = malloc(matrix_size * sizeof(int*));
    int **tree_ps = malloc(matrix_size * sizeof(int*));
    for (int i = 0; i < matrix_size; i++) {
        rock_ps[i] = calloc(matrix_size, sizeof(int));
        tree_ps[i] = calloc(matrix_size, sizeof(int));
    }

    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            int rock = (matrix[i * matrix_size + j] == 2);
            int tree = (matrix[i * matrix_size + j] == 1);

            rock_ps[i][j] = rock;
            tree_ps[i][j] = tree;

            if (i > 0) {
                rock_ps[i][j] += rock_ps[i-1][j];
                tree_ps[i][j] += tree_ps[i-1][j];
            }
            if (j > 0) {
                rock_ps[i][j] += rock_ps[i][j-1];
                tree_ps[i][j] += tree_ps[i][j-1];
            }
            if (i > 0 && j > 0) {
                rock_ps[i][j] -= rock_ps[i-1][j-1];
                tree_ps[i][j] -= tree_ps[i-1][j-1];
            }
        }
    }

    for (int top = 0; top <= matrix_size - park_size; top++) {
        for (int left = 0; left <= matrix_size - park_size; left++) {
            int bottom = top + park_size - 1;
            int right  = left + park_size - 1;

            int cx1 = top + center_edge_offset;
            int cy1 = left + center_edge_offset;
            int cx2 = cx1 + center_block_size - 1;
            int cy2 = cy1 + center_block_size - 1;

            int rock_count = prefix_sum(rock_ps, cx1, cy1, cx2, cy2);

            if (rock_count >= min_rock_num) {
                int tree_count = prefix_sum(tree_ps, top, left, bottom, right);
                if (tree_count > max_tree_count) {
                    max_tree_count = tree_count;
                }
            }
        }
    }

    for (int i = 0; i < matrix_size; i++) {
        free(rock_ps[i]);
        free(tree_ps[i]);
    }
    free(rock_ps);
    free(tree_ps);

    return max_tree_count;
}
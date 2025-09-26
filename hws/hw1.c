// Homework 1 - Park

#include <stdio.h>
#include <stdlib.h>

int matrix_size, park_size, center_edge_offset, min_rock_num;
char* load_input();
void print_matrix();

int main() {
    char * matrix = load_input();
    print_matrix(matrix, matrix_size);
    fprintf(stdout, "matrix size: %d\n", matrix_size);
}

char* load_input() {
    if (fscanf(stdin, "%d %d %d %d",
               &matrix_size, &park_size,
               &center_edge_offset, &min_rock_num) != 4) {
        fprintf(stderr, "Chyba: nepodarilo se nacist 4 parametry.\n");
        exit(100);
    }

    printf("Input was loaded\n");
    printf("matrix size: %d\n", matrix_size);
    printf("park_size: %d\n", park_size);
    printf("center_edge_offset: %d\n", center_edge_offset);
    printf("min_rock_num: %d\n", min_rock_num);

    char *main_matrix = malloc(matrix_size * matrix_size * sizeof(char));
    if (!main_matrix) {
        fprintf(stderr, "Chyba: malloc selhal.\n");
        exit(102);
    }

    for (int idx = 0; idx < matrix_size; idx++) {
        for (int idy = 0; idy < matrix_size; idy++) {
            if (fscanf(stdin, "%d", (int*)&main_matrix[idx * matrix_size + idy]) != 1) {
                fprintf(stderr, "Chyba: nepodarilo se nacist prvek matice.\n");
                exit(101);
            }
        }
    }

    return main_matrix;
}

void print_matrix(const char *matrix, int matrix_size) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%d ", matrix[i * matrix_size + j]);
        }
        printf("\n");
    }
}
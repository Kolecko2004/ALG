#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 12000
#define MAX_NODE_VAL 2000

int nodes_num;

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int value;
} Node;

int* load_values() {
    fscanf(stdin, "%d", &nodes_num);

    int* node_arr = malloc(nodes_num * sizeof(int));

    for (int idx = 0; idx < nodes_num; idx++) {
        fscanf(stdin, "%d", &node_arr[idx]);
    }

    return node_arr;
}

void print_input(int* arr) {
    fprintf(stdout, "%d\n", nodes_num);

    for (int idx = 0; idx < nodes_num; idx++) {
        fprintf(stdout, "%d, ", arr[idx]);
    }

    fprintf(stdout, "\n");
}

void make_tree() {
    Node* node = malloc(sizeof(Node));
    
}

int main() {
    int* node_arr = load_values();

    // print_input(node_arr);

    return 0;
}
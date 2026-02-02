#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 120000
#define MAX_NODE_VAL 2000

int nodes_num;
int num_disabled = 0;

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int value;
    int idx;
    int disabled;
    int max_down_val;
} Node;

int* load_values() {
    fscanf(stdin, "%d", &nodes_num);

    int* val_arr = malloc(nodes_num * sizeof(int));

    for (int idx = 0; idx < nodes_num; idx++) {
        fscanf(stdin, "%d", &val_arr[idx]);
    }

    return val_arr;
}

void print_input(int* arr) {
    fprintf(stdout, "%d\n", nodes_num);

    for (int idx = 0; idx < nodes_num; idx++) {
        fprintf(stdout, "%d, ", arr[idx]);
    }

    fprintf(stdout, "\n");
}

Node* make_tree(int idx, int* val_arr, Node** all_nodes) {
    if (idx >= nodes_num)
        return NULL;

    Node* node = malloc(sizeof(Node));
    node->value = val_arr[idx];
    node->idx = idx;
    node->disabled = 0;
    node->left = make_tree(2 * idx + 1, val_arr, all_nodes);
    node->right = make_tree(2 * idx + 2, val_arr, all_nodes);

    all_nodes[idx] = node;
    return node;
}

int best_path(Node* node, int* val_arr) {
    if (node == NULL || node->disabled)
        return 0;

    int sum_left = best_path(node->left, val_arr);
    int sum_right = best_path(node->right, val_arr);

    // This updates the array with the sum of the "Best Chain" passing through this node
    val_arr[node->idx] = sum_left + sum_right + node->value;

    // Returns the max single branch going down
    return node->value + (sum_left > sum_right ? sum_left : sum_right);
}

void delete_entire_subtree(Node* node, int* val_arr) {
    if (node == NULL || node->disabled)
        return;

    delete_entire_subtree(node->left, val_arr);
    delete_entire_subtree(node->right, val_arr);

    node->disabled = 1;
    val_arr[node->idx] = -1;
}

// Logic to find which way the "best path" went and delete it
void delete_all(Node* node, int* val_arr) {
    if (node == NULL || node->disabled)
        return;

    int L = (node->left && !node->left->disabled) ? best_path(node->left, val_arr) : -1;
    int R = (node->right && !node->right->disabled) ? best_path(node->right, val_arr) : -1;

    node->disabled = 1;
    val_arr[node->idx] = -1;

    if (L >= R && L != -1)
        delete_all(node->left, val_arr);
    else if (R != -1)
        delete_all(node->right, val_arr);
}

void find_and_delete(Node* node, int target_idx, int* val_arr) {
    if (node == NULL)
        return;
    if (node->idx == target_idx) {
        // When we find the "peak" node, delete its best left branch AND best right branch
        if (node->left)
            delete_all(node->left, val_arr);
        if (node->right)
            delete_all(node->right, val_arr);
        node->disabled = 1;
        val_arr[node->idx] = -1;
        return;
    }
    find_and_delete(node->left, target_idx, val_arr);
    find_and_delete(node->right, target_idx, val_arr);
}

int main() {
    int* val_arr = load_values();
    if (!val_arr)
        return 0;

    Node** all_nodes = malloc(sizeof(Node*) * nodes_num);
    Node* root = make_tree(0, val_arr, all_nodes);

    int num = 0;
    while (1) {
        // Reset and recalculate sums
        best_path(root, val_arr);

        int max = 0;
        int idy = -1;
        for (int idx = 0; idx < nodes_num; idx++) {
            if (!all_nodes[idx]->disabled && val_arr[idx] > max) {
                max = val_arr[idx];
                idy = idx;
            }
        }

        if (idy == -1 || max <= 0)
            break;

        num++;
        find_and_delete(root, idy, val_arr);
    }

    // free(root);
    fprintf(stdout, "%d\n", num);

    return 0;
}
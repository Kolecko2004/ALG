#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    struct Node *left, *right;
    int value;
    int idx;
    int disabled;
    int max_down_val; // Helper for DP
} Node;

int nodes_num;

// 1. Fixed Boundary Check (idx >= nodes_num)
Node* make_tree(int idx, int* node_val_arr, Node** all_nodes) {
    if (idx >= nodes_num) return NULL;

    Node* node = malloc(sizeof(Node));
    node->value = node_val_arr[idx];
    node->idx = idx;
    node->disabled = 0;
    node->max_down_val = 0;
    
    node->left = make_tree(2 * idx + 1, node_val_arr, all_nodes);
    node->right = make_tree(2 * idx + 2, node_val_arr, all_nodes);

    all_nodes[idx] = node;
    return node;
}

// 2. Correct Path Value Calculation (Dynamic Programming)
// Returns the max path value that starts at 'node' and goes DOWN.
int calculate_max_down(Node* node) {
    if (node == NULL || node->disabled) return 0;
    
    int L = calculate_max_down(node->left);
    int R = calculate_max_down(node->right);
    
    node->max_down_val = node->value + (L > R ? L : R);
    return node->max_down_val;
}

// 3. Finding the "Bending" Path
// A chain can be LeftSubtree + Node + RightSubtree
int get_full_chain_value(Node* node) {
    if (node == NULL || node->disabled) return 0;
    int L = (node->left && !node->left->disabled) ? node->left->max_down_val : 0;
    int R = (node->right && !node->right->disabled) ? node->right->max_down_val : 0;
    return node->value + L + R;
}

// 4. Clean Deletion of a Path
void disable_path_down(Node* node) {
    if (node == NULL || node->disabled) return;
    
    int L = (node->left && !node->left->disabled) ? node->left->max_down_val : 0;
    int R = (node->right && !node->right->disabled) ? node->right->max_down_val : 0;
    
    node->disabled = 1;
    
    // Follow the path that gave us the max value
    if (L >= R && L > 0) disable_path_down(node->left);
    else if (R > 0) disable_path_down(node->right);
}

void delete_best_chain(Node* top_node) {
    int L = (top_node->left && !top_node->left->disabled) ? top_node->left->max_down_val : 0;
    int R = (top_node->right && !top_node->right->disabled) ? top_node->right->max_down_val : 0;
    
    top_node->disabled = 1;
    if (L > 0) disable_path_down(top_node->left);
    if (R > 0) disable_path_down(top_node->right);
}

int main() {
    if (fscanf(stdin, "%d", &nodes_num) != 1) return 0;

    int* val_arr = malloc(sizeof(int) * nodes_num);
    for (int i = 0; i < nodes_num; i++) fscanf(stdin, "%d", &val_arr[i]);

    // Allocation on Heap to avoid Stack Overflow
    Node** all_nodes = malloc(sizeof(Node*) * nodes_num);
    Node* root = make_tree(0, val_arr, all_nodes);

    int path_count = 0;

    while (1) {
        // Step A: Update all max_down values (Bottom-up)
        calculate_max_down(root);

        // Step B: Find the node that acts as the 'peak' of the best chain
        int best_val = 0;
        int best_idx = -1;

        for (int i = 0; i < nodes_num; i++) {
            if (all_nodes[i]->disabled) continue;
            
            int current_chain = get_full_chain_value(all_nodes[i]);
            if (current_chain > best_val) {
                best_val = current_chain;
                best_idx = i;
            }
        }

        if (best_idx == -1 || best_val == 0) break;

        // Step C: Remove that chain and increment counter
        delete_best_chain(all_nodes[best_idx]);
        path_count++;
    }

    printf("%d\n", path_count);

    // Free memory (omitted for brevity, but recommended)
    return 0;
}
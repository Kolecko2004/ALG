#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char color;
    struct Node *left;
    struct Node *right;
    int idx;
} Node;

int num_pearls;
char *pearls = NULL;
int max_pearl = 0;
int current_idx = 0;

Node* create_node(char color, int idx) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->color = color;
    node->left = NULL;
    node->right = NULL;
    node->idx = idx;
    return node;
}

Node* build_tree() {
    if (current_idx >= num_pearls) {
        return NULL;
    }

    char color = pearls[current_idx];
    Node *node = create_node(color, current_idx);
    current_idx++;
    
    // If it's a leaf node (color '2'), it has no children
    if (color == '2') {
        return node;
    }
    
    // Otherwise, build left and right subtrees
    node->left = build_tree();
    node->right = build_tree();
    
    return node;
}

// Returns the maximum depth from this node, considering the constraint
// consecutive_count tracks how many consecutive same-colored nodes we've seen
int depth_and_update_diameter(Node *node, char parent_color, int consecutive_count) {
    if (node == NULL) {
        return 0;
    }

    // Check if we're extending a sequence of same colors
    int new_consecutive = (node->color == parent_color) ? consecutive_count + 1 : 1;
    
    // If we have 3 consecutive same colors, this path is invalid
    if (new_consecutive > 2) {
        return 0;
    }

    int left_depth = 0, right_depth = 0;

    if (node->left != NULL) {
        left_depth = depth_and_update_diameter(node->left, node->color, new_consecutive);
    }

    if (node->right != NULL) {
        right_depth = depth_and_update_diameter(node->right, node->color, new_consecutive);
    }

    // Update max path (diameter) through this node
    // Only count if both endpoints would be valid (reaching '2' nodes)
    int path_length = left_depth + right_depth + 1;
    if (path_length > max_pearl) {
        max_pearl = path_length;
    }

    // Return the longer path for use by parent
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void free_tree(Node *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main() {
    scanf("%d", &num_pearls);
    
    pearls = malloc(num_pearls * sizeof(char));
    for (int i = 0; i < num_pearls; i++) {
        scanf(" %c", &pearls[i]);
    }

    current_idx = 0;
    Node *root = build_tree();
    
    // Start with parent_color = -1 (no parent) and consecutive_count = 0
    depth_and_update_diameter(root, -1, 0);
    
    printf("%d\n", max_pearl);

    free_tree(root);
    free(pearls);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char color;
    struct Node *left;
    struct Node *right;
} Node;

int num_pearls;
char *pearls = NULL;
int max_pearl = 0;

Node* create_node(char color) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->color = color;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* build_tree(int idx) {
    if (idx >= num_pearls) {
        return NULL;
    }

    Node *node = create_node(pearls[idx]);
    
    if (pearls[idx] != '2') {
        node->left = build_tree(2 * idx + 1);
        node->right = build_tree(2 * idx + 2);
    }
    
    return node;
}

int depth_and_update_diameter(Node *node, char parent) {
    if (node == NULL) {
        return 0;
    }

    int left_depth = 0, right_depth = 0;

    if (node->left != NULL) {
        if (parent == node->color && node->left->color == node->color) {
            left_depth = depth_and_update_diameter(node->left, -1);
        } else {
            left_depth = depth_and_update_diameter(node->left, node->color);
        }
    }

    if (node->right != NULL) {
        if (parent == node->color && node->right->color == node->color) {
            right_depth = depth_and_update_diameter(node->right, -1);
        } else {
            right_depth = depth_and_update_diameter(node->right, node->color);
        }
    }

    int path_length = left_depth + right_depth + 1;
    if (path_length > max_pearl) {
        max_pearl = path_length;
    }

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

    Node *root = build_tree(0);
    depth_and_update_diameter(root, -1);
    
    printf("%d\n", max_pearl);

    free_tree(root);
    free(pearls);
    return 0;
}
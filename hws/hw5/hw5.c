#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    int deleted;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    int compact_count;
} BST;

Node* create_node(int key) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->deleted = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert_helper(Node *node, int key, int *changed) {
    if (node == NULL) {
        *changed = 1;
        return create_node(key);
    }
    
    if (key == node->key) {
        if (node->deleted) {
            node->deleted = 0;
            *changed = 1;
        }
        return node;
    }
    
    if (key < node->key) {
        node->left = insert_helper(node->left, key, changed);
    } else {
        node->right = insert_helper(node->right, key, changed);
    }
    
    return node;
}

void insert(BST *bst, int key) {
    int changed = 0;
    bst->root = insert_helper(bst->root, key, &changed);
}

Node* delete_helper(Node *node, int key, int *changed) {
    if (node == NULL) return NULL;
    
    if (key == node->key) {
        if (!node->deleted) {
            node->deleted = 1;
            *changed = 1;
        }
        return node;
    }
    
    if (key < node->key) {
        node->left = delete_helper(node->left, key, changed);
    } else {
        node->right = delete_helper(node->right, key, changed);
    }
    
    return node;
}

void delete_key(BST *bst, int key) {
    int changed = 0;
    bst->root = delete_helper(bst->root, key, &changed);
}

void compute_heights(Node *node, int depth, int tree_depth, int *deleted_sum, int *active_sum) {
    if (node == NULL) return;
    
    int height = tree_depth - depth;
    
    if (node->deleted) {
        *deleted_sum += height;
    } else {
        *active_sum += height;
    }
    
    compute_heights(node->left, depth + 1, tree_depth, deleted_sum, active_sum);
    compute_heights(node->right, depth + 1, tree_depth, deleted_sum, active_sum);
}

int get_depth(Node *node) {
    if (node == NULL) return -1;
    
    int left_depth = get_depth(node->left);
    int right_depth = get_depth(node->right);
    
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

int should_compact(Node *root) {
    if (root == NULL) return 0;
    
    int tree_depth = get_depth(root);
    int deleted_sum = 0;
    int active_sum = 0;
    
    compute_heights(root, 0, tree_depth, &deleted_sum, &active_sum);
    
    return deleted_sum > active_sum;
}

void collect_preorder(Node *node, int *keys, int *count) {
    if (node == NULL) return;
    
    if (!node->deleted) {
        keys[(*count)++] = node->key;
    }
    
    collect_preorder(node->left, keys, count);
    collect_preorder(node->right, keys, count);
}

void free_tree(Node *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

Node* build_from_preorder(int *keys, int start, int end) {
    if (start > end) return NULL;
    
    Node *root = create_node(keys[start]);
    
    // Find split point
    int split = start + 1;
    while (split <= end && keys[split] < keys[start]) {
        split++;
    }
    
    root->left = build_from_preorder(keys, start + 1, split - 1);
    root->right = build_from_preorder(keys, split, end);
    
    return root;
}

void compact(BST *bst) {
    if (bst->root == NULL) return;
    
    int *keys = (int*)malloc(250000 * sizeof(int));
    int count = 0;
    
    collect_preorder(bst->root, keys, &count);
    free_tree(bst->root);
    
    if (count == 0) {
        bst->root = NULL;
    } else {
        bst->root = build_from_preorder(keys, 0, count - 1);
    }
    
    free(keys);
    bst->compact_count++;
}

int main() {
    int n;
    scanf("%d", &n);
    
    BST bst;
    bst.root = NULL;
    bst.compact_count = 0;
    
    for (int i = 0; i < n; i++) {
        char op[4];
        int key;
        scanf("%s %d", op, &key);
        
        if (strcmp(op, "ins") == 0) {
            insert(&bst, key);
        } else {
            delete_key(&bst, key);
        }
        
        if (should_compact(bst.root)) {
            compact(&bst);
        }
    }
    
    int depth = get_depth(bst.root);
    printf("%d %d\n", bst.compact_count, depth);
    
    free_tree(bst.root);
    
    return 0;
}
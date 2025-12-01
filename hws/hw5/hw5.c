#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int deleted;
    struct Node *left;
    struct Node *right;
} Node;

static Node node_pool[250000];
static int pool_idx = 0;
static int keys_buffer[250000];

Node* alloc_node(int key) {
    Node *node = &node_pool[pool_idx++];
    node->key = key;
    node->deleted = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert_helper(Node *node, int key, int *changed) {
    if (!node) {
        *changed = 1;
        return alloc_node(key);
    }
    
    if (key == node->key) {
        if (node->deleted) {
            node->deleted = 0;
            *changed = 1;
        }
        return node;
    }
    
    if (key < node->key)
        node->left = insert_helper(node->left, key, changed);
    else
        node->right = insert_helper(node->right, key, changed);
    
    return node;
}

Node* delete_helper(Node *node, int key, int *changed) {
    if (!node) return NULL;
    
    if (key == node->key) {
        if (!node->deleted) {
            node->deleted = 1;
            *changed = 1;
        }
        return node;
    }
    
    if (key < node->key)
        node->left = delete_helper(node->left, key, changed);
    else
        node->right = delete_helper(node->right, key, changed);
    
    return node;
}

int get_depth(Node *node) {
    if (!node) return -1;
    int left = get_depth(node->left);
    int right = get_depth(node->right);
    return 1 + (left > right ? left : right);
}

void compute_sums(Node *node, int depth, int tree_depth, int *del_sum, int *act_sum) {
    if (!node) return;
    
    int height = tree_depth - depth;
    
    if (node->deleted)
        *del_sum += height;
    else
        *act_sum += height;
    
    compute_sums(node->left, depth + 1, tree_depth, del_sum, act_sum);
    compute_sums(node->right, depth + 1, tree_depth, del_sum, act_sum);
}

int should_compact(Node *root) {
    if (!root) return 0;
    
    int tree_depth = get_depth(root);
    int del_sum = 0, act_sum = 0;
    
    compute_sums(root, 0, tree_depth, &del_sum, &act_sum);
    
    return del_sum > act_sum;
}

void collect_preorder(Node *node, int *cnt) {
    if (!node) return;
    
    if (!node->deleted)
        keys_buffer[(*cnt)++] = node->key;
    
    collect_preorder(node->left, cnt);
    collect_preorder(node->right, cnt);
}

Node* build_from_preorder(int start, int end) {
    if (start > end) return NULL;
    
    Node *root = alloc_node(keys_buffer[start]);
    
    int split = start + 1;
    while (split <= end && keys_buffer[split] < keys_buffer[start])
        split++;
    
    root->left = build_from_preorder(start + 1, split - 1);
    root->right = build_from_preorder(split, end);
    
    return root;
}

Node* compact(Node *root, int *compact_cnt) {
    if (!root) return NULL;
    
    int cnt = 0;
    collect_preorder(root, &cnt);
    
    (*compact_cnt)++;
    
    if (cnt == 0)
        return NULL;
    
    return build_from_preorder(0, cnt - 1);
}

int main() {
    int n;
    scanf("%d", &n);
    
    Node *root = NULL;
    int compact_count = 0;
    
    char op[4];
    int key, changed;
    
    for (int i = 0; i < n; i++) {
        scanf("%s %d", op, &key);
        
        changed = 0;
        if (op[0] == 'i') {
            root = insert_helper(root, key, &changed);
        } else {
            root = delete_helper(root, key, &changed);
        }
        
        if (changed && should_compact(root)) {
            root = compact(root, &compact_count);
        }
    }
    
    int depth = get_depth(root);
    printf("%d %d\n", compact_count, depth);
    
    return 0;
}
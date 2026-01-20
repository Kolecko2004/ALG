#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 10000000

typedef struct Node {
    int keys[2];
    int num_keys;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

typedef struct {
    Node *root;
    int total_keys;
    int total_nodes;
} BBST;

Node *node_pool;
int pool_index = 0;

Node* create_node() {
    Node *n = &node_pool[pool_index++];
    n->num_keys = 0;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}

void init_bbst(BBST *tree) {
    tree->root = NULL;
    tree->total_keys = 0;
    tree->total_nodes = 0;
}

int min_key(Node *n) {
    return n->keys[0];
}

int max_key(Node *n) {
    return n->keys[n->num_keys - 1];
}

int has_key(Node *n, int key) {
    for (int i = 0; i < n->num_keys; i++) {
        if (n->keys[i] == key) return 1;
    }
    return 0;
}

Node* find_node(BBST *tree, int key) {
    Node *curr = tree->root;
    while (curr) {
        if (has_key(curr, key)) return curr;
        if (key < min_key(curr)) curr = curr->left;
        else if (key > max_key(curr)) curr = curr->right;
        else return NULL;
    }
    return NULL;
}

void insert_key(BBST *tree, int key) {
    if (find_node(tree, key)) return;
    
    if (!tree->root) {
        tree->root = create_node();
        tree->root->keys[0] = key;
        tree->root->num_keys = 1;
        tree->total_keys++;
        tree->total_nodes++;
        return;
    }
    
    Node *curr = tree->root;
    while (1) {
        if (key < min_key(curr)) {
            if (!curr->left) {
                curr->left = create_node();
                curr->left->parent = curr;
                curr->left->keys[0] = key;
                curr->left->num_keys = 1;
                tree->total_keys++;
                tree->total_nodes++;
                return;
            }
            curr = curr->left;
        } else if (key > max_key(curr)) {
            if (!curr->right) {
                curr->right = create_node();
                curr->right->parent = curr;
                curr->right->keys[0] = key;
                curr->right->num_keys = 1;
                tree->total_keys++;
                tree->total_nodes++;
                return;
            }
            curr = curr->right;
        } else {
            int old_max = max_key(curr);
            curr->keys[1] = key;
            curr->num_keys = 2;
            if (curr->keys[0] > curr->keys[1]) {
                int tmp = curr->keys[0];
                curr->keys[0] = curr->keys[1];
                curr->keys[1] = tmp;
            }
            tree->total_keys++;
            insert_key(tree, old_max);
            return;
        }
    }
}

Node* find_min_node(Node *n) {
    while (n->left) n = n->left;
    return n;
}

void delete_key(BBST *tree, int key) {
    Node *x = find_node(tree, key);
    if (!x) return;
    
    if (x->num_keys == 2) {
        if (x->keys[0] == key) {
            x->keys[0] = x->keys[1];
        }
        x->num_keys = 1;
        tree->total_keys--;
        return;
    }
    
    int num_children = (x->left ? 1 : 0) + (x->right ? 1 : 0);
    
    if (num_children == 0) {
        if (x->parent) {
            if (x->parent->left == x) x->parent->left = NULL;
            else x->parent->right = NULL;
        } else {
            tree->root = NULL;
        }
        tree->total_keys--;
        tree->total_nodes--;
    } else if (num_children == 1) {
        Node *child = x->left ? x->left : x->right;
        if (!x->parent) {
            tree->root = child;
            child->parent = NULL;
        } else {
            if (x->parent->left == x) x->parent->left = child;
            else x->parent->right = child;
            child->parent = x->parent;
        }
        tree->total_keys--;
        tree->total_nodes--;
    } else {
        Node *min_node = find_min_node(x->right);
        int replacement = min_key(min_node);
        x->keys[0] = replacement;
        delete_key(tree, replacement);
    }
}

int get_depth(Node *n) {
    if (!n) return -1;
    int left_depth = get_depth(n->left);
    int right_depth = get_depth(n->right);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void collect_keys(Node *n, int *keys, int *index) {
    if (!n) return;
    collect_keys(n->left, keys, index);
    for (int i = 0; i < n->num_keys; i++) {
        keys[(*index)++] = n->keys[i];
    }
    collect_keys(n->right, keys, index);
}

// Build a perfectly balanced BBST from sorted keys
// Returns the node and sets *keys_used to number of keys consumed
Node* build_consolidated(int *keys, int num_keys, int *pos, Node *parent) {
    if (num_keys == 0) return NULL;
    
    Node *node = create_node();
    node->parent = parent;
    
    if (num_keys == 1) {
        node->keys[0] = keys[(*pos)++];
        node->num_keys = 1;
        return node;
    }
    
    if (num_keys == 2) {
        node->keys[0] = keys[(*pos)++];
        node->keys[1] = keys[(*pos)++];
        node->num_keys = 2;
        return node;
    }
    
    if (num_keys == 3) {
        node->keys[0] = keys[(*pos)++];
        node->num_keys = 1;
        node->left = build_consolidated(keys, 1, pos, node);
        node->right = build_consolidated(keys, 1, pos, node);
        return node;
    }
    
    // For num_keys >= 4
    // Calculate the structure: we want a nearly complete binary tree
    // where all nodes except root have 2 keys
    
    // Number of nodes needed
    int nodes_needed = (num_keys + 1) / 2; // ceil(num_keys / 2)
    
    // Find the height and structure
    int h = 0;
    int temp = nodes_needed;
    while (temp > 1) {
        temp /= 2;
        h++;
    }
    
    // Calculate left subtree size
    int max_last_level = 1 << h; // 2^h
    // int max_nodes = (1 << (h + 1)) - 1; // 2^(h+1) - 1
    int last_level = nodes_needed - ((1 << h) - 1);
    
    int left_subtree_nodes;
    if (last_level <= max_last_level / 2) {
        left_subtree_nodes = (1 << (h - 1)) - 1 + last_level;
    } else {
        left_subtree_nodes = (1 << h) - 1;
    }
    
    int left_keys = left_subtree_nodes * 2;
    
    // Root gets 2 keys for even total, 1 key for odd total
    int root_keys = (num_keys % 2 == 0) ? 2 : 1;
    
    // Adjust if not enough keys
    if (left_keys > num_keys - root_keys) {
        left_keys = num_keys - root_keys - (num_keys - root_keys - left_keys);
    }
    
    int right_keys = num_keys - root_keys - left_keys;
    
    node->left = build_consolidated(keys, left_keys, pos, node);
    
    node->keys[0] = keys[(*pos)++];
    if (root_keys == 2) {
        node->keys[1] = keys[(*pos)++];
    }
    node->num_keys = root_keys;
    
    node->right = build_consolidated(keys, right_keys, pos, node);
    
    return node;
}

int count_nodes(Node *n) {
    if (!n) return 0;
    return 1 + count_nodes(n->left) + count_nodes(n->right);
}

void consolidate(BBST *tree) {
    if (!tree->root || tree->total_keys == 0) return;
    
    int *keys = (int*)malloc(tree->total_keys * sizeof(int));
    int index = 0;
    collect_keys(tree->root, keys, &index);
    
    int n = index;
    pool_index = 0;
    
    int pos = 0;
    tree->root = build_consolidated(keys, n, &pos, NULL);
    tree->total_nodes = count_nodes(tree->root);
    tree->total_keys = n;
    
    free(keys);
}

int main() {
    long long A, B, M, N, OP;
    scanf("%lld %lld %lld %lld %lld", &A, &B, &M, &N, &OP);
    
    node_pool = (Node*)malloc(MAX_KEYS * sizeof(Node));
    
    BBST tree;
    init_bbst(&tree);
    
    long long s = B;
    for (long long i = 0; i < N; i++) {
        insert_key(&tree, (int)s);
        s = (A * s + B) % M;
    }
    
    for (long long i = 0; i < OP; i++) {
        char op[10];
        scanf("%s", op);
        
        if (op[0] == 'C') {
            printf("%d %d %d\n", tree.total_keys, tree.total_nodes, get_depth(tree.root));
            consolidate(&tree);
        } else if (op[0] == 'I') {
            int key;
            scanf("%d", &key);
            insert_key(&tree, key);
        } else if (op[0] == 'D') {
            int key;
            scanf("%d", &key);
            delete_key(&tree, key);
        }
    }
    
    free(node_pool);
    return 0;
}
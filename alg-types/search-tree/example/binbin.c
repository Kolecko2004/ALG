#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int keys[2];
    int numKeys;
    int left, right;
} Node;

Node pool[6000005]; 
int poolPtr = 0;
int sortedKeys[3000005];
int keyCount = 0;
int readPtr = 0;

int createNode() {
    int idx = poolPtr++;
    pool[idx].numKeys = 0;
    pool[idx].left = -1;
    pool[idx].right = -1;
    return idx;
}

// --- STANDARDNÍ OPERACE (ZŮSTÁVAJÍ STEJNÉ) ---

int insert(int idx, int key) {
    if (idx == -1) {
        int n = createNode();
        pool[n].keys[0] = key; pool[n].numKeys = 1;
        return n;
    }
    Node *n = &pool[idx];
    if (key == n->keys[0] || (n->numKeys == 2 && key == n->keys[1])) return idx;
    if (key < n->keys[0]) n->left = insert(n->left, key);
    else if (n->numKeys == 1 || key > n->keys[1]) n->right = insert(n->right, key);
    else {
        int oldMax = n->keys[1];
        n->keys[1] = key;
        n->right = insert(n->right, oldMax);
    }
    return idx;
}

int getMin(int idx) {
    while (pool[idx].left != -1) idx = pool[idx].left;
    return pool[idx].keys[0];
}

int deleteKey(int idx, int key) {
    if (idx == -1) return -1;
    Node *n = &pool[idx];
    if (n->numKeys == 2) {
        if (n->keys[0] == key) { n->keys[0] = n->keys[1]; n->numKeys = 1; return idx; }
        if (n->keys[1] == key) { n->numKeys = 1; return idx; }
    }
    if (n->numKeys == 1 && n->keys[0] == key) {
        if (n->left == -1 && n->right == -1) return -1;
        if (n->left == -1) return n->right;
        if (n->right == -1) return n->left;
        int successor = getMin(n->right);
        n->keys[0] = successor;
        n->right = deleteKey(n->right, successor);
        return idx;
    }
    if (key < n->keys[0]) n->left = deleteKey(n->left, key);
    else n->right = deleteKey(n->right, key);
    return idx;
}

void collectKeys(int idx) {
    if (idx == -1) return;
    collectKeys(pool[idx].left);
    sortedKeys[keyCount++] = pool[idx].keys[0];
    if (pool[idx].numKeys == 2) sortedKeys[keyCount++] = pool[idx].keys[1];
    collectKeys(pool[idx].right);
}

void getStats(int idx, int currentDepth, int *nodes, int *maxDepth) {
    if (idx == -1) return;
    (*nodes)++;
    if (currentDepth > *maxDepth) *maxDepth = currentDepth;
    getStats(pool[idx].left, currentDepth + 1, nodes, maxDepth);
    getStats(pool[idx].right, currentDepth + 1, nodes, maxDepth);
}

// --- OPRAVENÁ KONSOLIDACE ---

/* Klíčem je vypočítat, kolik klíčů půjde do levého podstromu, 
   aby byla dodržena pravidla o hloubce a zaplnění zleva.
*/
int get_left_keys(int n, int is_root) {
    if (n <= 0) return 0;
    int k = (is_root && (n % 2 != 0)) ? 1 : 2;
    if (n == 1) return 0;
    if (n == 2) return 0;
    
    int remaining = n - k;
    // Výpočet kapacity pro ideálně vyvážený strom se 2 klíči na uzel
    // Podstromy BBST rostou mocninami 3 (každý uzel má 3 potenciální cesty)
    int h = 0;
    long long capacity = 2; // Počet klíčů v jedné hladině (pokud jsou všechny uzly plné)
    long long total_cap = 0;
    
    // Zjistíme výšku podstromu
    while (total_cap + capacity < remaining) {
        total_cap += capacity;
        capacity *= 3;
    }
    
    // Rozdělíme zbývající klíče tak, aby se levý podstrom plnil prioritně
    int half = (remaining + 1) / 2;
    return half;
}

int buildOptimal(int n, int is_root) {
    if (n <= 0) return -1;
    
    int k = (is_root && (n % 2 != 0)) ? 1 : 2;
    if (n == 1) k = 1;
    if (n == 2) k = 2;

    int remaining = n - k;
    int left_n = (remaining + 1) / 2;
    int right_n = remaining - left_n;

    // Musíme nejdříve postavit levou stranu, pak vzít klíče pro uzel, pak pravou
    int left_idx = buildOptimal(left_n, 0);
    
    int idx = createNode();
    pool[idx].numKeys = k;
    pool[idx].left = left_idx;
    pool[idx].keys[0] = sortedKeys[readPtr++];
    if (k == 2) pool[idx].keys[1] = sortedKeys[readPtr++];
    
    pool[idx].right = buildOptimal(right_n, 0);
    
    return idx;
}

int main() {
    long long A, B, M, N, OP;
    if (scanf("%lld %lld %lld %lld %lld", &A, &B, &M, &N, &OP) != 5) return 0;

    int root = -1;
    long long currentS = B;
    root = insert(root, (int)currentS);
    for (int i = 1; i < N; i++) {
        currentS = (A * currentS + B) % M;
        root = insert(root, (int)currentS);
    }

    char cmd[10];
    int val;
    while (OP--) {
        if (scanf("%s", cmd) != 1) break;
        if (cmd[0] == 'C') {
            int nodes = 0, depth = 0;
            keyCount = 0;
            getStats(root, 0, &nodes, &depth);
            
            int total = 0;
            collectKeys(root);
            total = keyCount;
            printf("%d %d %d\n", total, nodes, depth);

            poolPtr = 0;
            readPtr = 0;
            root = buildOptimal(total, 1);
        } else if (cmd[0] == 'I') {
            scanf("%d", &val);
            root = insert(root, val);
        } else if (cmd[0] == 'D') {
            scanf("%d", &val);
            root = deleteKey(root, val);
        }
    }
    return 0;
}
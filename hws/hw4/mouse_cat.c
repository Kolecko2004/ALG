#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 200005
#define INF 1000000000

typedef struct Edge {
    int to;
    struct Edge* next;
} Edge;

typedef struct {
    unsigned char* bits;
    int size;
} BitSet;

// Stav pro BFS
typedef struct {
    int mouse_pos;
    int cat_jumps;
    BitSet cat_set;
    int dist;
    int noisy;
} State;

// Prioritní fronta (heapová struktura)
typedef struct {
    State* data;
    int size;
    int capacity;
} PriorityQueue;

Edge* adj[MAX_N];
bool noisy[MAX_N];
int n, h, e, S, C, K, M;

// BitSet operace
BitSet createBitSet(int n) {
    BitSet bs;
    bs.size = (n + 7) / 8;
    bs.bits = (unsigned char*)calloc(bs.size, sizeof(unsigned char));
    return bs;
}

void setBit(BitSet* bs, int pos) {
    bs->bits[pos / 8] |= (1 << (pos % 8));
}

bool getBit(BitSet* bs, int pos) {
    return (bs->bits[pos / 8] & (1 << (pos % 8))) != 0;
}

void clearBitSet(BitSet* bs) {
    memset(bs->bits, 0, bs->size);
}

void copyBitSet(BitSet* dest, BitSet* src) {
    memcpy(dest->bits, src->bits, src->size);
}

BitSet cloneBitSet(BitSet* src) {
    BitSet bs;
    bs.size = src->size;
    bs.bits = (unsigned char*)malloc(bs.size);
    memcpy(bs.bits, src->bits, bs.size);
    return bs;
}

void freeBitSet(BitSet* bs) {
    if (bs->bits) free(bs->bits);
}

// Porovnání pro prioritu: (distance, noisy_count)
int compareStates(State* a, State* b) {
    if (a->dist != b->dist) return a->dist - b->dist;
    return a->noisy - b->noisy;
}

// Prioritní fronta
PriorityQueue* createPQ(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->data = (State*)malloc(capacity * sizeof(State));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void pqSwap(State* a, State* b) {
    State temp = *a;
    *a = *b;
    *b = temp;
}

void pqHeapifyUp(PriorityQueue* pq, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (compareStates(&pq->data[idx], &pq->data[parent]) < 0) {
            pqSwap(&pq->data[idx], &pq->data[parent]);
            idx = parent;
        } else break;
    }
}

void pqHeapifyDown(PriorityQueue* pq, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < pq->size && compareStates(&pq->data[left], &pq->data[smallest]) < 0)
            smallest = left;
        if (right < pq->size && compareStates(&pq->data[right], &pq->data[smallest]) < 0)
            smallest = right;
            
        if (smallest != idx) {
            pqSwap(&pq->data[idx], &pq->data[smallest]);
            idx = smallest;
        } else break;
    }
}

void pqPush(PriorityQueue* pq, State s) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->data = (State*)realloc(pq->data, pq->capacity * sizeof(State));
    }
    pq->data[pq->size] = s;
    pqHeapifyUp(pq, pq->size);
    pq->size++;
}

State pqPop(PriorityQueue* pq) {
    State result = pq->data[0];
    pq->size--;
    if (pq->size > 0) {
        pq->data[0] = pq->data[pq->size];
        pqHeapifyDown(pq, 0);
    }
    return result;
}

bool pqEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

// Hash pro memoizaci
typedef struct HashNode {
    int mouse;
    int jumps;
    unsigned long long hash_cats;
    int best_dist;
    int best_noisy;
    struct HashNode* next;
} HashNode;

#define HASH_SIZE 2000003
HashNode* hash_table[HASH_SIZE];

unsigned long long hashBitSet(BitSet* bs) {
    unsigned long long h = 0;
    for (int i = 0; i < bs->size && i < 8; i++) {
        h = h * 256 + bs->bits[i];
    }
    return h;
}

unsigned long long computeHash(int mouse, int jumps, unsigned long long cat_hash) {
    return (((unsigned long long)mouse * 100000ULL + jumps) * 1000000007ULL + cat_hash) % HASH_SIZE;
}

bool checkAndUpdate(int mouse, int jumps, BitSet* cats, int dist, int noisy) {
    unsigned long long cat_hash = hashBitSet(cats);
    unsigned long long h = computeHash(mouse, jumps, cat_hash);
    
    HashNode* curr = hash_table[h];
    while (curr) {
        if (curr->mouse == mouse && curr->jumps == jumps && curr->hash_cats == cat_hash) {
            if (dist < curr->best_dist || (dist == curr->best_dist && noisy < curr->best_noisy)) {
                curr->best_dist = dist;
                curr->best_noisy = noisy;
                return true;
            }
            return false;
        }
        curr = curr->next;
    }
    
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    node->mouse = mouse;
    node->jumps = jumps;
    node->hash_cats = cat_hash;
    node->best_dist = dist;
    node->best_noisy = noisy;
    node->next = hash_table[h];
    hash_table[h] = node;
    return true;
}

void addEdge(int u, int v) {
    Edge* e = (Edge*)malloc(sizeof(Edge));
    e->to = v;
    e->next = adj[u];
    adj[u] = e;
}

void solve() {
    PriorityQueue* pq = createPQ(10000);
    
    State init;
    init.mouse_pos = S;
    init.cat_jumps = 0;
    init.cat_set = createBitSet(n);
    setBit(&init.cat_set, K);
    init.dist = 0;
    init.noisy = 0;
    
    pqPush(pq, init);
    
    int result_dist = INF;
    int result_noisy = INF;
    
    while (!pqEmpty(pq)) {
        State curr = pqPop(pq);
        
        // Optimalizace: pokud už máme lepší, skip
        if (curr.dist > result_dist || 
            (curr.dist == result_dist && curr.noisy >= result_noisy)) {
            freeBitSet(&curr.cat_set);
            continue;
        }
        
        // Dosažení cíle
        if (curr.mouse_pos == C) {
            if (curr.dist < result_dist || 
                (curr.dist == result_dist && curr.noisy < result_noisy)) {
                result_dist = curr.dist;
                result_noisy = curr.noisy;
            }
            freeBitSet(&curr.cat_set);
            continue;
        }
        
        // Zkontroluj, zda jsme už navštívili lepší stav
        if (!checkAndUpdate(curr.mouse_pos, curr.cat_jumps, &curr.cat_set, curr.dist, curr.noisy)) {
            freeBitSet(&curr.cat_set);
            continue;
        }
        
        // Prozkoumej sousedy
        for (Edge* e = adj[curr.mouse_pos]; e != NULL; e = e->next) {
            int next_pos = e->to;
            
            State next;
            next.mouse_pos = next_pos;
            next.dist = curr.dist + 1;
            next.noisy = curr.noisy + (noisy[next_pos] ? 1 : 0);
            
            // Expanduj pozice kocoura
            if (noisy[next_pos] && curr.cat_jumps < M) {
                next.cat_jumps = curr.cat_jumps + 1;
                next.cat_set = createBitSet(n);
                
                for (int cat_pos = 0; cat_pos < n; cat_pos++) {
                    if (!getBit(&curr.cat_set, cat_pos)) continue;
                    
                    setBit(&next.cat_set, cat_pos);  // Zůstat
                    for (Edge* ce = adj[cat_pos]; ce != NULL; ce = ce->next) {
                        setBit(&next.cat_set, ce->to);  // Skočit
                    }
                }
            } else {
                next.cat_jumps = curr.cat_jumps;
                next.cat_set = cloneBitSet(&curr.cat_set);
            }
            
            // Kontrola bezpečnosti
            if (getBit(&next.cat_set, next_pos)) {
                freeBitSet(&next.cat_set);
                continue;
            }
            
            pqPush(pq, next);
        }
        
        freeBitSet(&curr.cat_set);
    }
    
    printf("Result: %d %d\n", result_dist, result_noisy);
}

int main() {
    scanf("%d %d %d", &n, &h, &e);
    scanf("%d %d %d %d", &S, &C, &K, &M);
    
    memset(noisy, false, sizeof(noisy));
    for (int i = 0; i < h; i++) {
        int node;
        scanf("%d", &node);
        noisy[node] = true;
    }
    
    memset(adj, 0, sizeof(adj));
    for (int i = 0; i < e; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(u, v);
        addEdge(v, u);
    }
    
    solve();
    
    return 0;
}
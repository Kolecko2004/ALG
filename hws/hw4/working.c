#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NODES 200005
#define INFINITY_DISTANCE 1000000000
#define HASH_TABLE_SIZE 2000003

typedef struct Edge {
    int destination;
    struct Edge* next;
} Edge;

typedef struct {
    unsigned char* bits;
    int byteSize;
} BitSet;

typedef struct {
    int mouse_pos;
    int cat_jump_num;
    BitSet poss_cat_position;
    int dist_from_start;
    int noisy_node_num;
} SearchState;

typedef struct {
    SearchState* states;
    int size;
    int capacity;
} PriorityQueue;

typedef struct HashNode {
    int mouse_pos;
    int cat_jump_num;
    unsigned long long cat_pos_hash;
    int best_dist;
    int best_noice_num;
    struct HashNode* next;
} HashNode;

Edge* adjacencyList[MAX_NODES];
bool isNoisyNode[MAX_NODES];
HashNode* memoizationTable[HASH_TABLE_SIZE];

int totalNodes, noisyNodeCount, edgeCount;
int mouseStartNode, mouseGoalNode, catStartNode, maxCatJumps;

BitSet create_bit_set(int n) {
    BitSet bitset;
    bitset.byteSize = (n + 7) / 8;
    bitset.bits = (unsigned char*)calloc(bitset.byteSize, sizeof(unsigned char));
    return bitset;
}

void set_bit(BitSet* bitset, int position) {
    bitset->bits[position / 8] |= (1 << (position % 8));
}

bool get_bit(BitSet* bitset, int position) {
    return (bitset->bits[position / 8] & (1 << (position % 8))) != 0;
}

void clear_bit_set(BitSet* bitset) {
    memset(bitset->bits, -1, bitset->byteSize);
}

void copy_bit_set(BitSet* destination, BitSet* source) {
    memcpy(destination->bits, source->bits, source->byteSize);
}

BitSet clone_bit_set(BitSet* source) {
    BitSet bitset;
    bitset.byteSize = source->byteSize;
    bitset.bits = (unsigned char*)malloc(bitset.byteSize);
    memcpy(bitset.bits, source->bits, bitset.byteSize);
    return bitset;
}

void free_bit_set(BitSet* bitset) {
    if (bitset->bits) {
        free(bitset->bits);
    }
}

int compare_states(SearchState* a, SearchState* b) {
    if (a->dist_from_start != b->dist_from_start) {
        return a->dist_from_start - b->dist_from_start;
    }
    return a->noisy_node_num - b->noisy_node_num;
}

PriorityQueue* create_priority_queue(int initialCapacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->states = (SearchState*)malloc(initialCapacity * sizeof(SearchState));
    pq->size = 0;
    pq->capacity = initialCapacity;
    return pq;
}

void swap_states(SearchState* a, SearchState* b) {
    SearchState temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (compare_states(&pq->states[index], &pq->states[parentIndex]) < 0) {
            swap_states(&pq->states[index], &pq->states[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void heapify_down(PriorityQueue* pq, int index) {
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;
        
        if (leftChild < pq->size && 
            compare_states(&pq->states[leftChild], &pq->states[smallest]) < 0) {
            smallest = leftChild;
        }
        if (rightChild < pq->size && 
            compare_states(&pq->states[rightChild], &pq->states[smallest]) < 0) {
            smallest = rightChild;
        }
            
        if (smallest != index) {
            swap_states(&pq->states[index], &pq->states[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void push(PriorityQueue* pq, SearchState state) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->states = (SearchState*)realloc(pq->states, pq->capacity * sizeof(SearchState));
    }
    
    pq->states[pq->size] = state;
    heapify_up(pq, pq->size);
    pq->size++;
}

SearchState pop(PriorityQueue* pq) {
    SearchState result = pq->states[0];
    pq->size--;
    
    if (pq->size > 0) {
        pq->states[0] = pq->states[pq->size];
        heapify_down(pq, 0);
    }
    
    return result;
}

bool is_priority_queue_empty(PriorityQueue* pq) {
    return pq->size == 0;
}

unsigned long long hash_bit_set(BitSet* bitset) {
    unsigned long long hash = 0;
    int bytesToHash = bitset->byteSize < 8 ? bitset->byteSize : 8;
    
    for (int i = 0; i < bytesToHash; i++) {
        hash = hash * 256 + bitset->bits[i];
    }
    
    return hash;
}

unsigned long long compute_hash_index(int mousePos, int jumps, unsigned long long catHash) {
    return (((unsigned long long)mousePos * 100000ULL + jumps) * 1000000007ULL + catHash) 
           % HASH_TABLE_SIZE;
}

bool should_explore_state(int mousePos, int catJumps, BitSet* catPositions, 
                        int distance, int noisyCount) {
    unsigned long long catHash = hash_bit_set(catPositions);
    unsigned long long hashIndex = compute_hash_index(mousePos, catJumps, catHash);
    
    HashNode* current = memoizationTable[hashIndex];
    while (current != NULL) {
        if (current->mouse_pos == mousePos && 
            current->cat_jump_num == catJumps && 
            current->cat_pos_hash == catHash) {
            
            if (distance < current->best_dist || 
                (distance == current->best_dist && noisyCount < current->best_noice_num)) {
                current->best_dist = distance;
                current->best_noice_num = noisyCount;
                return true;
            }
            return false;
        }
        current = current->next;
    }
    
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->mouse_pos = mousePos;
    newNode->cat_jump_num = catJumps;
    newNode->cat_pos_hash = catHash;
    newNode->best_dist = distance;
    newNode->best_noice_num = noisyCount;
    newNode->next = memoizationTable[hashIndex];
    memoizationTable[hashIndex] = newNode;
    
    return true;
}

void add_edge(int fromNode, int toNode) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->destination = toNode;
    edge->next = adjacencyList[fromNode];
    adjacencyList[fromNode] = edge;
}

void find_path() {
    PriorityQueue* pq = create_priority_queue(10000);
    
    SearchState initialState;
    initialState.mouse_pos = mouseStartNode;
    initialState.cat_jump_num = 0;
    initialState.poss_cat_position = create_bit_set(totalNodes);
    set_bit(&initialState.poss_cat_position, catStartNode);
    initialState.dist_from_start = 0;
    initialState.noisy_node_num = 0;
    
    push(pq, initialState);
    
    int best_dist = INFINITY_DISTANCE;
    int best_noice_num = INFINITY_DISTANCE;
    
    while (!is_priority_queue_empty(pq)) {
        SearchState currentState = pop(pq);
        
        if (currentState.dist_from_start > best_dist || 
            (currentState.dist_from_start == best_dist && 
             currentState.noisy_node_num >= best_noice_num)) {
            free_bit_set(&currentState.poss_cat_position);
            continue;
        }
        
        if (currentState.mouse_pos == mouseGoalNode) {
            if (currentState.dist_from_start < best_dist || 
                (currentState.dist_from_start == best_dist && 
                 currentState.noisy_node_num < best_noice_num)) {
                best_dist = currentState.dist_from_start;
                best_noice_num = currentState.noisy_node_num;
            }
            free_bit_set(&currentState.poss_cat_position);
            continue;
        }
        
        if (!should_explore_state(currentState.mouse_pos, 
                               currentState.cat_jump_num,
                               &currentState.poss_cat_position,
                               currentState.dist_from_start,
                               currentState.noisy_node_num)) {
            free_bit_set(&currentState.poss_cat_position);
            continue;
        }
        
        for (Edge* edge = adjacencyList[currentState.mouse_pos]; 
             edge != NULL; 
             edge = edge->next) {
            
            int nextmouse_pos = edge->destination;
            
            SearchState nextState;
            nextState.mouse_pos = nextmouse_pos;
            nextState.dist_from_start = currentState.dist_from_start + 1;
            nextState.noisy_node_num = currentState.noisy_node_num + 
                                         (isNoisyNode[nextmouse_pos] ? 1 : 0);
            
            if (isNoisyNode[nextmouse_pos] && currentState.cat_jump_num < maxCatJumps) {
                nextState.cat_jump_num = currentState.cat_jump_num + 1;
                nextState.poss_cat_position = create_bit_set(totalNodes);
                
                for (int catPos = 0; catPos < totalNodes; catPos++) {
                    if (!get_bit(&currentState.poss_cat_position, catPos)) {
                        continue;
                    }
                    
                    set_bit(&nextState.poss_cat_position, catPos);
                    
                    for (Edge* catEdge = adjacencyList[catPos]; 
                         catEdge != NULL; 
                         catEdge = catEdge->next) {
                        set_bit(&nextState.poss_cat_position, catEdge->destination);
                    }
                }
            } else {
                nextState.cat_jump_num = currentState.cat_jump_num;
                nextState.poss_cat_position = clone_bit_set(&currentState.poss_cat_position);
            }
            
            if (get_bit(&nextState.poss_cat_position, nextmouse_pos)) {
                free_bit_set(&nextState.poss_cat_position);
                continue;
            }
            
            push(pq, nextState);
        }
        
        free_bit_set(&currentState.poss_cat_position);
    }
    
    fprintf(stdout, "%d %d\n", best_dist, best_noice_num);
}

int main() {
    fscanf(stdin, "%d %d %d", &totalNodes, &noisyNodeCount, &edgeCount);
    fscanf(stdin, "%d %d %d %d", &mouseStartNode, &mouseGoalNode, &catStartNode, &maxCatJumps);
    
    memset(isNoisyNode, false, sizeof(isNoisyNode));
    for (int i = 0; i < noisyNodeCount; i++) {
        int nodeNumber;
        fscanf(stdin, "%d", &nodeNumber);
        isNoisyNode[nodeNumber] = true;
    }
    
    memset(adjacencyList, 0, sizeof(adjacencyList));
    for (int i = 0; i < edgeCount; i++) {
        int u, v;
        fscanf(stdin, "%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }
    
    find_path();
    
    return 0;
}
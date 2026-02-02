#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Node {
    int id;
    struct Node** neighbors;
    int neighbor_count;
    int arr_capacity;
    int component_id;
} Node;

typedef struct {
    Node* node;
    int dist;
} QueueItem;

int n_start, n_end, D;
int num_nodes = 0;
Node* all_nodes = NULL;
int num_of_components = 0;

void make_graph() {
    for (int idx = 0; idx < num_nodes; idx++) {
        all_nodes[idx].id = n_start + idx;
        all_nodes[idx].neighbor_count = 0;
        all_nodes[idx].arr_capacity = 10;
        all_nodes[idx].component_id = -1;
        all_nodes[idx].neighbors = (Node**) malloc(all_nodes[idx].arr_capacity * sizeof(Node*));
    }
}

void add_edge(Node* node1, Node* node2) {
    if (node1->neighbor_count >= node1->arr_capacity) {
        node1->arr_capacity *= 2;
        node1->neighbors = realloc(node1->neighbors, node1->arr_capacity * sizeof(Node*));
    }
    node1->neighbors[node1->neighbor_count++] = node2;
}

long long nsd(long long a, long long b) {
    while (b) {
        a %= b;
        long long tmp = a;
        a = b;
        b = tmp;
    }
    return a;
}

void load_input() {
    if (fscanf(stdin, "%d %d %d", &n_start, &n_end, &D) != 3)
        return;
    num_nodes = n_end - n_start + 1;
    all_nodes = (Node*) malloc(num_nodes * sizeof(Node));
}

void is_edge(Node* node1, Node* node2) {
    long long a = MAX(node1->id, node2->id);
    long long b = MIN(node1->id, node2->id);
    if (nsd(2 * a + 3, a * b + a - b + 2) >= D) {
        add_edge(node1, node2);
        add_edge(node2, node1);
    }
}

void make_edges() {
    for (int idx = 0; idx < num_nodes; idx++) {
        for (int idy = idx + 1; idy < num_nodes; idy++) {
            is_edge(&all_nodes[idx], &all_nodes[idy]);
        }
    }
}

// ITERATIVNÍ hledání komponent (místo rekurze, aby nebyl SegFault)
void find_component_iterative(int start_idx) {
    int* stack = malloc(num_nodes * sizeof(int));
    int top = -1;

    stack[++top] = start_idx;
    all_nodes[start_idx].component_id = num_of_components;

    while (top >= 0) {
        int u_idx = stack[top--];
        Node* u = &all_nodes[u_idx];
        for (int i = 0; i < u->neighbor_count; i++) {
            int v_idx = u->neighbors[i]->id - n_start;
            if (all_nodes[v_idx].component_id == -1) {
                all_nodes[v_idx].component_id = num_of_components;
                stack[++top] = v_idx;
            }
        }
    }
    free(stack);
}

// Opravené BFS pro vzdálenosti
int get_eccentricity(Node* start_node) {
    int* dists = malloc(num_nodes * sizeof(int));
    for (int i = 0; i < num_nodes; i++)
        dists[i] = -1;

    QueueItem* queue = malloc(num_nodes * sizeof(QueueItem));
    int head = 0, tail = 0;

    int start_idx = start_node->id - n_start;
    dists[start_idx] = 0;
    queue[tail++] = (QueueItem){start_node, 0};

    int max_dist = 0;
    while (head < tail) {
        QueueItem current = queue[head++];
        if (current.dist > max_dist)
            max_dist = current.dist;

        for (int i = 0; i < current.node->neighbor_count; i++) {
            Node* neighbor = current.node->neighbors[i];
            int n_idx = neighbor->id - n_start;
            if (dists[n_idx] == -1) {
                dists[n_idx] = current.dist + 1;
                queue[tail++] = (QueueItem){neighbor, current.dist + 1};
            }
        }
    }
    free(dists);
    free(queue);
    return max_dist;
}

void process_graph() {
    int global_max_radius = 0;
    // 1. Najdi komponenty
    for (int i = 0; i < num_nodes; i++) {
        if (all_nodes[i].component_id == -1) {
            find_component_iterative(i);
            num_of_components++;
        }
    }

    // 2. Spočítej poloměry
    for (int c = 0; c < num_of_components; c++) {
        int min_ecc = 1e9;
        int comp_size = 0;
        for (int i = 0; i < num_nodes; i++) {
            if (all_nodes[i].component_id == c) {
                comp_size++;
                int ecc = get_eccentricity(&all_nodes[i]);
                if (ecc < min_ecc)
                    min_ecc = ecc;
            }
        }
        if (comp_size > 1 && min_ecc > global_max_radius)
            global_max_radius = min_ecc;
    }
    printf("%d %d\n", num_of_components, global_max_radius);
}

int main() {
    load_input();
    if (num_nodes <= 0)
        return 0;
    make_graph();
    make_edges();
    process_graph();
    return 0;
}
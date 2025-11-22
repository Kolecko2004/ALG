#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NODES 200005
#define INFINITY_DISTANCE 1000000000

typedef struct Edge {
    int to;
    struct Edge* next;
} Edge;

typedef struct {
    int mouse_pos;
    int noisy_cnt;
    int dist;
} State;

typedef struct {
    State* data;
    int front, rear, size, capacity;
} Queue;

Edge* adj[MAX_NODES];
bool noisy[MAX_NODES];
int total_nodes, noisy_node_count, edge_count, mouse_start_node, mouse_goal_node, cat_start_node, maxC_cat_jumps;

int dist_cat[MAX_NODES];
int visited[MAX_NODES][55];

Queue* create_queue(int cap) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q -> data = (State*)malloc(cap * sizeof(State));
    q -> front = 0;
    q -> rear = 0;
    q -> size = 0;
    q -> capacity = cap;
    return q;
}

void enqueue(Queue* q, State s) {
    if (q -> size >= q -> capacity) {

        int old_cap = q -> capacity;
        q -> capacity *= 2;
        State* new_data = (State*)malloc(q -> capacity * sizeof(State));

        for (int i = 0; i < q -> size; i++) {
            new_data[i] = q -> data[(q -> front + i) % old_cap];
        }

        free(q -> data);
        q -> data = new_data;
        q -> front = 0;
        q -> rear = q -> size;
    }

    q -> data[q -> rear] = s;
    q -> rear = (q -> rear + 1) % q -> capacity;
    q -> size++;
}

State dequeue(Queue* q) {
    State s = q -> data[q -> front];
    q -> front = (q -> front + 1) % q -> capacity;
    q -> size--;
    return s;
}

bool is_empty(Queue* q) {
    return q -> size == 0;
}

void add_edge(int u, int v) {
    Edge* e = (Edge*)malloc(sizeof(Edge));
    e -> to = v;
    e -> next = adj[u];
    adj[u] = e;
}

void bfs_cat() {
    for (int i = 0; i < total_nodes; i++) {
        dist_cat[i] = INFINITY_DISTANCE;
    }

    int queue[MAX_NODES];
    int head = 0, tail = 0;

    queue[tail++] = cat_start_node;
    dist_cat[cat_start_node] = 0;

    while (head < tail) {
        int u = queue[head++];

        for (Edge* e = adj[u]; e; e = e -> next) {
            int v = e -> to;
            if (dist_cat[v] == INFINITY_DISTANCE) {
                dist_cat[v] = dist_cat[u] + 1;
                queue[tail++] = v;
            }
        }
    }
}

void solve() {
    bfs_cat();

    for (int i = 0; i < total_nodes; i++) {
        for (int t = 0; t <= maxC_cat_jumps; t++) {
            visited[i][t] = -1;
        }
    }

    Queue* q = create_queue(10000);

    State init;
    init.mouse_pos = mouse_start_node;
    init.noisy_cnt = 0;
    init.dist = 0;

    visited[mouse_start_node][0] = 0;
    enqueue(q, init);

    int best_dist = INFINITY_DISTANCE;
    int best_noisy = INFINITY_DISTANCE;

    while (!is_empty(q)) {
        State curr = dequeue(q);

        if (curr.dist > best_dist) continue;
        if (curr.dist == best_dist && curr.noisy_cnt >= best_noisy) continue;

        if (curr.mouse_pos == mouse_goal_node) {
            if (curr.dist < best_dist ||
                (curr.dist == best_dist && curr.noisy_cnt < best_noisy)) {
                best_dist = curr.dist;
                best_noisy = curr.noisy_cnt;
            }
            continue;
        }

        for (Edge* e = adj[curr.mouse_pos]; e; e = e -> next) {
            int v = e -> to;

            int newNoisy = curr.noisy_cnt + (noisy[v] ? 1 : 0);

            if (newNoisy > maxC_cat_jumps) {
                newNoisy = maxC_cat_jumps;
            }

            if (dist_cat[v] <= newNoisy) continue;

            if (visited[v][newNoisy] != -1) continue;

            visited[v][newNoisy] = curr.dist + 1;

            State nxt;
            nxt.mouse_pos = v;
            nxt.noisy_cnt = newNoisy;
            nxt.dist = curr.dist + 1;

            enqueue(q, nxt);
        }
    }

    fprintf(stdout, "%d %d\n", best_dist, best_noisy);
}

int main() {
    fscanf(stdin, "%d %d %d", &total_nodes, &noisy_node_count, &edge_count);
    fscanf(stdin, "%d %d %d %d", &mouse_start_node, &mouse_goal_node, &cat_start_node, &maxC_cat_jumps);

    memset(noisy, false, sizeof(noisy));
    for (int i = 0; i < noisy_node_count; i++) {
        int node;
        fscanf(stdin, "%d", &node);
        noisy[node] = true;
    }

    memset(adj, 0, sizeof(adj));
    for (int i = 0; i < edge_count; i++) {
        int u, v;
        fscanf(stdin, "%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }

    solve();
    return 0;
}

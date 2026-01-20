#include <stdio.h>

#define MAX_VERTICES 10

// adjacency list
int graph[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int vertices;

/* ===== DFS ===== */
void dfs(int v) {
    visited[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < vertices; i++) {
        if (graph[v][i] && !visited[i]) {
            dfs(i);
        }
    }
}

/* ===== BFS ===== */
void bfs(int start) {
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int v = queue[front++];
        printf("%d ", v);

        for (int i = 0; i < vertices; i++) {
            if (graph[v][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

int main() {
    vertices = 6;

    // ručně definovaný graf (neorientovaný)
    int edges[][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}};

    int edgeCount = sizeof(edges) / sizeof(edges[0]);

    // inicializace grafu
    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            graph[i][j] = 0;

    // přidání hran
    for (int i = 0; i < edgeCount; i++) {
        int a = edges[i][0];
        int b = edges[i][1];
        graph[a][b] = 1;
        graph[b][a] = 1;
    }

    printf("DFS od 0: ");
    for (int i = 0; i < vertices; i++)
        visited[i] = 0;
    dfs(0);

    printf("\nBFS od 0: ");
    for (int i = 0; i < vertices; i++)
        visited[i] = 0;
    bfs(0);

    printf("\n");
    return 0;
}

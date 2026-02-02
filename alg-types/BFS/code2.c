#include <stdio.h>

// Using fixed-size static arrays for maximum cache locality
static int adj[1001][1001];
static int degree[1001];
static int comp_id[1001];
static int dist[1001];
static int queue[1001];
static int nodes_in_comp[1001];
static long long ids[1001];

// Faster Binary GCD Algorithm (Steiner's Algorithm)
static inline long long fast_gcd(long long a, long long b) {
    if (!a || !b) return a | b;
    int shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do {
        b >>= __builtin_ctzll(b);
        if (a > b) { long long t = a; a = b; b = t; }
        b -= a;
    } while (b);
    return a << shift;
}

int main() {
    long long n_start, n_end, D;
    if (scanf("%lld %lld %lld", &n_start, &n_end, &D) != 3) return 0;

    int n = (int)(n_end - n_start + 1);

    // 1. Build IDs and Edges
    for (int i = 0; i < n; i++) {
        ids[i] = n_start + i;
        comp_id[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        long long a = ids[i];
        long long x = 2 * a + 3; // Precomputed arg1
        for (int j = i + 1; j < n; j++) {
            long long b = ids[j];
            // arg2 simplified: a*b + a - b + 2
            if (fast_gcd(x, a * b + a - b + 2) >= D) {
                adj[i][degree[i]++] = j;
                adj[j][degree[j]++] = i;
            }
        }
    }

    // 2. Iterative DFS for Components
    int num_comps = 0;
    for (int i = 0; i < n; i++) {
        if (comp_id[i] != -1) continue;
        
        int head = 0, tail = 0;
        queue[tail++] = i;
        comp_id[i] = num_comps;
        while(head < tail) {
            int u = queue[head++];
            for(int k = 0; k < degree[u]; k++) {
                int v = adj[u][k];
                if (comp_id[v] == -1) {
                    comp_id[v] = num_comps;
                    queue[tail++] = v;
                }
            }
        }
        num_comps++;
    }

    // 3. Radius Calculation with Early Exits
    int max_global_radius = 0;
    for (int c = 0; c < num_comps; c++) {
        int comp_size = 0;
        for (int i = 0; i < n; i++) {
            if (comp_id[i] == c) nodes_in_comp[comp_size++] = i;
        }

        if (comp_size <= 1) continue;

        int min_ecc = 1001;
        for (int i = 0; i < comp_size; i++) {
            int start_node = nodes_in_comp[i];
            
            // Fast BFS
            for (int k = 0; k < n; k++) dist[k] = -1;
            int qh = 0, qt = 0;
            queue[qt++] = start_node;
            dist[start_node] = 0;
            int current_ecc = 0;

            while(qh < qt) {
                int u = queue[qh++];
                if (dist[u] > current_ecc) current_ecc = dist[u];
                
                // If this node is already worse than our best candidate, stop BFS early
                if (current_ecc >= min_ecc) break;

                for(int k = 0; k < degree[u]; k++) {
                    int v = adj[u][k];
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        queue[qt++] = v;
                    }
                }
            }
            if (current_ecc < min_ecc) min_ecc = current_ecc;
            if (min_ecc == 1) break; // Diameter 2 components have radius 1
        }
        if (min_ecc > max_global_radius) max_global_radius = min_ecc;
    }

    printf("%d %d\n", num_comps, max_global_radius);
    return 0;
}
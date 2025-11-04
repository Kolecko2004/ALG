#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int count_bits(unsigned int x) {
    int c = 0;
    while (x) {
        if (x & 1u) c++;
        x >>= 1;
    }
    return c;
}

int lowest_bit_index(unsigned int x) {
    int i = 0;
    while ((x & 1u) == 0u) {
        x >>= 1;
        i++;
    }
    return i;
}

int main(void) {
    int n, m, a, b;
    if (scanf("%d %d %d %d", &n, &m, &a, &b) != 4) return 0;
    int k = a + b;
    if (k > n) k = n;

    unsigned int mask[30] = {0};
    int deg[30] = {0};
    int *adj[30];
    for (int i = 0; i < n; i++)
        adj[i] = malloc(n * sizeof(int));

    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--; v--;
        mask[u] |= (1u << v);
        mask[v] |= (1u << u);
        adj[u][deg[u]++] = v;
        adj[v][deg[v]++] = u;
    }

    unsigned int limit = (n == 32) ? 0xFFFFFFFFu : ((1u << n) - 1u);
    unsigned int s = (k == 0) ? 0 : ((1u << k) - 1u);

    unsigned long long best_sum = 0;

    while (s && (s & limit) == s) {
        int delta[30];
        int dcnt = 0;
        unsigned long long base = 0ULL;

        unsigned int t = s;
        while (t) {
            int v = lowest_bit_index(t);
            t &= (t - 1);

            int in = count_bits(mask[v] & s);
            int out = deg[v] - in;
            base += out;
            delta[dcnt++] = 2 * in - deg[v];
        }

        long long add = 0;
        if (a > 0) {
            for (int i = 0; i < a; i++) {
                int best_j = i;
                for (int j = i + 1; j < dcnt; j++)
                    if (delta[j] > delta[best_j])
                        best_j = j;
                int tmp = delta[i];
                delta[i] = delta[best_j];
                delta[best_j] = tmp;
                add += delta[i];
            }
        }

        unsigned long long total = base + add;
        if (total > best_sum) best_sum = total;

        unsigned int c = s & (~s + 1);
        unsigned int r = s + c;
        if (r == 0) break;
        s = (((r ^ s) >> 2) / c) | r;
    }

    printf("%llu\n", best_sum);

    for (int i = 0; i < n; i++)
        free(adj[i]);
    return 0;
}

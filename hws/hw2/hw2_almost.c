// hw2.c
// Přesné řešení optimalizace rozmístění agentů (N <= 30).
// Kompilace: gcc -O2 -std=c11 hw2.c -o hw2

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int popcount_u32(uint32_t x) { return __builtin_popcount(x); }

int cmp_desc(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ib - ia;
}

int main(void) {
    int N, M, A, B;
    if (scanf("%d %d %d %d", &N, &M, &A, &B) != 4) return 0;
    int K = A + B;
    // adjacency list (to match požadavku) and adjacency bitmask
    int *adj_list[30];
    int deg[30];
    for (int i = 0; i < N; ++i) {
        deg[i] = 0;
        adj_list[i] = malloc(N * sizeof(int)); // max N-1 neighbors
    }

    uint32_t adjmask[30];
    for (int i = 0; i < N; ++i) adjmask[i] = 0;

    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        --u; --v;
        adj_list[u][deg[u]++] = v;
        adj_list[v][deg[v]++] = u;
        adjmask[u] |= (1u << v);
        adjmask[v] |= (1u << u);
    }

    // Special cases
    if (K == 0) { // no agents -> score 0
        printf("0\n");
        for (int i = 0; i < N; ++i) free(adj_list[i]);
        return 0;
    }

    // Enumerate all subsets S of size K using Gosper's hack over N bits.
    // We'll generate initial mask with K low bits set, then shift within N bits.
    uint64_t max_score = 0;
    if (K > N) K = N; // defensive

    // start combination: lowest K bits set
    uint32_t comb;
    if (K == 32) comb = 0xFFFFFFFFu;
    else comb = (K == 0) ? 0u : ((1u << K) - 1u);

    uint32_t limit = (N == 32) ? 0xFFFFFFFFu : ((1u << N) - 1u);

    // iterate all combinations of K bits within first N bits
    for (uint32_t s = comb; s <= limit; ) {
        // compute baseline (all in S chosen as T2): sum c2 = sum_v (deg[v] - c1(v))
        // and collect delta d(v)=2*c1 - deg for v in S
        int delt[30];
        int di = 0;
        uint64_t baseline = 0;
        // iterate vertices present in s
        uint32_t t = s;
        while (t) {
            int v = __builtin_ctz(t);
            t &= t - 1;
            int c1 = popcount_u32(adjmask[v] & s); // neighbors in S
            int c2 = deg[v] - c1;
            baseline += c2;
            delt[di++] = 2 * c1 - deg[v];
        }
        // choose top A deltas (di == K). If A==0, sum 0.
        uint64_t add = 0;
        if (A > 0) {
            // simple sort descending (K <= 30 so fine)
            qsort(delt, di, sizeof(int), cmp_desc);
            for (int i = 0; i < A; ++i) add += (uint64_t)delt[i];
        }
        uint64_t total = baseline + add;
        if (total > max_score) max_score = total;

        // Gosper's hack: next combination with same popcount
        if (s == 0) break;
        uint32_t c = s & -s;
        uint32_t r = s + c;
        if (r == 0) break;
        s = (((r ^ s) >> 2) / c) | r;
        // break condition: if s has bits outside N, stop
        if ((s & limit) != s) break;
    }

    printf("%llu\n", (unsigned long long)max_score);

    for (int i = 0; i < N; ++i) free(adj_list[i]);
    return 0;
}

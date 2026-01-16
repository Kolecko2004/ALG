#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int start;
    int end;
    unsigned char dir;
} seg_t;

int main() {
    int M, N;
    scanf("%d %d", &M, &N);
    int MN = M * N;

    int *a = malloc(sizeof(int) * MN);
    for (int i = 0; i < MN; i++) scanf("%d", &a[i]);

    int maxDim = M > N ? M : N;
    int WMAX = 100 * maxDim;

    int *bucket_count = calloc(WMAX + 1, sizeof(int));

    int **row_pref = malloc(sizeof(int*) * M);
    for (int r = 0; r < M; r++) {
        row_pref[r] = malloc(sizeof(int) * (N + 1));
        row_pref[r][0] = 0;
        for (int c = 0; c < N; c++)
            row_pref[r][c + 1] = row_pref[r][c] + a[r * N + c];
    }

    int **col_pref = malloc(sizeof(int*) * N);
    for (int c = 0; c < N; c++) {
        col_pref[c] = malloc(sizeof(int) * (M + 1));
        col_pref[c][0] = 0;
        for (int r = 0; r < M; r++)
            col_pref[c][r + 1] = col_pref[c][r] + a[r * N + c];
    }

    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            for (int c2 = c + 1; c2 < N; c2++) {
                int w = (c2 - c >= 2) ? row_pref[r][c2] - row_pref[r][c + 1] : 0;
                bucket_count[w]++;
            }
            for (int r2 = r + 1; r2 < M; r2++) {
                int w = (r2 - r >= 2) ? col_pref[c][r2] - col_pref[c][r + 1] : 0;
                bucket_count[w]++;
            }
        }
    }

    seg_t **buckets = malloc(sizeof(seg_t*) * (WMAX + 1));
    int *bucket_pos = calloc(WMAX + 1, sizeof(int));

    for (int w = 0; w <= WMAX; w++)
        buckets[w] = bucket_count[w] ? malloc(sizeof(seg_t) * bucket_count[w]) : NULL;

    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            int s = r * N + c;
            for (int c2 = c + 1; c2 < N; c2++) {
                int w = (c2 - c >= 2) ? row_pref[r][c2] - row_pref[r][c + 1] : 0;
                buckets[w][bucket_pos[w]++] = (seg_t){s, r * N + c2, 0};
            }
            for (int r2 = r + 1; r2 < M; r2++) {
                int w = (r2 - r >= 2) ? col_pref[c][r2] - col_pref[c][r + 1] : 0;
                buckets[w][bucket_pos[w]++] = (seg_t){s, r2 * N + c, 1};
            }
        }
    }

    for (int r = 0; r < M; r++) free(row_pref[r]);
    for (int c = 0; c < N; c++) free(col_pref[c]);
    free(row_pref);
    free(col_pref);

    long long NEG = LLONG_MIN / 4;
    long long *best = malloc(sizeof(long long) * MN * 2);
    for (int i = 0; i < MN * 2; i++) best[i] = NEG;

    int start = 0;
    int target = (M - 1) * N + (N - 1);

    for (int w = WMAX; w >= 0; w--) {
        int cnt = bucket_count[w];
        if (!cnt) continue;

        long long *tmp = malloc(sizeof(long long) * cnt);
        for (int i = 0; i < cnt; i++) {
            seg_t s = buckets[w][i];
            long long base = (s.start == start) ? 0 : best[s.start * 2 + (1 - s.dir)];
            tmp[i] = (base == NEG) ? NEG : base + w;
        }

        for (int i = 0; i < cnt; i++) {
            if (tmp[i] == NEG) continue;
            seg_t s = buckets[w][i];
            long long *ref = &best[s.end * 2 + s.dir];
            if (tmp[i] > *ref) *ref = tmp[i];
        }

        free(tmp);
        free(buckets[w]);
    }

    long long res = best[target * 2];
    if (best[target * 2 + 1] > res) res = best[target * 2 + 1];
    if (res < 0) res = 0;

    printf("%lld\n", res);

    free(best);
    free(bucket_pos);
    free(bucket_count);
    free(buckets);
    free(a);
    return 0;
}

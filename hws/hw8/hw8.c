#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int main() {
    int N;
    long long P, L;

    if (scanf("%d %lld %lld", &N, &P, &L) != 3)
        return 0;

    int *a = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i]);

    long long *pref = malloc((N + 1) * sizeof(long long));
    pref[0] = 0;
    for (int i = 0; i < N; i++)
        pref[i + 1] = pref[i] + a[i];

    long long INF = LLONG_MAX / 4;
    long long *costC = malloc(N * N * sizeof(long long));

    for (int i = 0; i < N * N; i++)
        costC[i] = INF;

    for (int i = 0; i < N; i++)
        costC[i * N + i] = 0;

    for (int len = 2; len <= N; len++) {
        for (int i = 0; i + len - 1 < N; i++) {
            int j = i + len - 1;
            long long best = INF;

            for (int k = i; k < j; k++) {
                long long leftSum  = pref[k + 1] - pref[i];
                long long rightSum = pref[j + 1] - pref[k + 1];
                long long add = L * llabs(leftSum - rightSum);

                long long cand =
                    costC[i * N + k] +
                    costC[(k + 1) * N + j] +
                    add;

                if (cand < best)
                    best = cand;
            }
            costC[i * N + j] = best;
        }
    }

    long long *dp = malloc((N + 1) * sizeof(long long));
    for (int i = 0; i <= N; i++)
        dp[i] = INF;

    dp[0] = 0;

    for (int j = 1; j <= N; j++) {
        for (int i = 0; i < j; i++) {
            long long cand = dp[i] + P + costC[i * N + (j - 1)];
            if (cand < dp[j])
                dp[j] = cand;
        }
    }

    long long profit = (long long)N * P - dp[N];
    printf("%lld\n", profit);

    free(a);
    free(pref);
    free(costC);
    free(dp);

    return 0;
}

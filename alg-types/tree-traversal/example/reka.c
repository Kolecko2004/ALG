#include <stdio.h>
#include <stdlib.h>

#define MAXN 1500005
#define INF 2000000

// Globální pole alokovaná v datovém segmentu (ne na stacku)
int parent[MAXN];
int children[MAXN][2];
int num_children[MAXN];
int out_degree[MAXN]; 
int dp[MAXN][4];
int queue[MAXN];

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    // Inicializace polí
    for (int i = 0; i <= N; i++) {
        num_children[i] = 0;
        out_degree[i] = 0;
        for (int k = 0; k < 4; k++) dp[i][k] = -INF;
    }

    // Načtení hran: C -> D (C je blíž k pramenu, D k moři)
    for (int i = 0; i < N; i++) {
        int c, d;
        if (scanf("%d %d", &c, &d) != 2) break;
        parent[c] = d;
        children[d][num_children[d]++] = c;
        out_degree[d]++;
    }

    // Najdeme listy pro post-order (zdroje tekutiny)
    int head = 0, tail = 0;
    for (int i = 0; i <= N; i++) {
        if (num_children[i] == 0) {
            queue[tail++] = i;
            dp[i][0] = 0; // Uzel i nepoužijeme jako pramen
            dp[i][1] = 0; // Uzel i použijeme jako pramen pro 1 tekutinu
        }
    }

    // Iterativní post-order DP (od listů ke kořeni)
    while (head < tail) {
        int u = queue[head++];
        if (u == 0) continue; // Ústí (kořen) nemá rodiče

        int p = parent[u];
        // Jakmile zpracujeme všechny děti uzlu p, můžeme spočítat jeho DP
        if (--out_degree[p] == 0) {
            queue[tail++] = p;
            
            int v1 = children[p][0];
            int v2 = (num_children[p] > 1) ? children[p][1] : -1;

            if (v2 == -1) { 
                // Uzel s jedním dítětem (jednoduchá sekce)
                for (int k = 0; k < 4; k++) {
                    if (dp[v1][k] >= 0) {
                        dp[p][k] = dp[v1][k] + (k == 1 ? 1 : 0);
                    }
                }
            } else { 
                // Soutok dvou řek (kombinujeme stavy obou dětí)
                for (int i = 0; i < 4; i++) {
                    if (dp[v1][i] < 0) continue;
                    for (int j = 0; j < 4 - i; j++) {
                        if (dp[v2][j] < 0) continue;
                        
                        int current_liquids = i + j;
                        // Skóre: zisk z podstromů + 1 bod za každou příchozí hranu s 1 tekutinou
                        int score = dp[v1][i] + (i == 1 ? 1 : 0) + 
                                    dp[v2][j] + (j == 1 ? 1 : 0);
                        
                        if (score > dp[p][current_liquids]) {
                            dp[p][current_liquids] = score;
                        }
                    }
                }
            }
        }
    }

    // Výsledek je v kořeni (bod 0) pro stav se 3 tekutinami
    printf("%d\n", dp[0][3]);

    return 0;
}
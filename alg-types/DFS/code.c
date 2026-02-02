#include <stdio.h>
#include <stdlib.h>

int M, N, a, b;
int r_start, s_start, r_target, s_target;
unsigned char **visited; // unsigned char šetří paměť (1 byte na buňku)

typedef struct {
    int r, s;
    int neighbor_idx; // Kterého souseda (0-3) se právě pokoušíme navštívit
} Frame;

int get_value(int r, int s) {
    return (int)(((1LL * r * N + s) * a) % b);
}

// Pořadí priorit: Doleva, Doprava, Nahoru, Dolů
int dr[] = {0, 0, -1, 1};
int ds[] = {-1, 1, 0, 0};

int main() {
    if (scanf("%d %d %d %d", &M, &N, &a, &b) != 4) return 0;
    if (scanf("%d %d %d %d", &r_start, &s_start, &r_target, &s_target) != 4) return 0;

    // Dynamická alokace visited matice
    visited = (unsigned char **)malloc(M * sizeof(unsigned char *));
    for (int i = 0; i < M; i++) {
        visited[i] = (unsigned char *)calloc(N, sizeof(unsigned char));
    }

    // Stack pro simulaci rekurze (maximální hloubka je M * N)
    Frame *stack = (Frame *)malloc(M * N * sizeof(Frame));
    int top = -1;

    long long steps = 0;
    // int found = 0;

    // Počáteční stav
    top++;
    stack[top] = (Frame){r_start, s_start, -1};
    visited[r_start][s_start] = 1;

    while (top >= 0) {
        int r = stack[top].r;
        int s = stack[top].s;

        // Kontrola cíle
        if (r == r_target && s == s_target) {
            // found = 1;
            break;
        }

        int best_val = -1;
        int best_dir = -1;

        // Najdeme nejlepšího (nejmenšího) dosud nenavštíveného souseda
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int ns = s + ds[i];

            if (nr >= 0 && nr < M && ns >= 0 && ns < N && !visited[nr][ns]) {
                int val = get_value(nr, ns);
                if (best_dir == -1 || val < best_val) {
                    best_val = val;
                    best_dir = i;
                }
            }
        }

        if (best_dir != -1) {
            // Jdeme o krok hlouběji (PUSH)
            steps++;
            int next_r = r + dr[best_dir];
            int next_s = s + ds[best_dir];
            
            visited[next_r][next_s] = 1;
            top++;
            stack[top] = (Frame){next_r, next_s, -1};
        } else {
            // Nikam už se nedá jít, vracíme se (POP)
            top--;
            if (top >= 0) {
                steps++; // Krok zpět se počítá
            }
        }
    }

    printf("%lld\n", steps);

    // Clean up
    for (int i = 0; i < M; i++) free(visited[i]);
    free(visited);
    free(stack);

    return 0;
}
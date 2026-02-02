// Shrnutí zadání "Synchronizace robotů" Označování polí
//     : Každé pole,
//       na kterém kdy jakýkoliv robot stál,
//       je navždy "označené" a nikdo na něj nesmí znovu vstoupit
//           .Hromadný příkaz : Dispečer řekne "Doleva" a všichni roboti se zkusí pohnout doleva.Ti,
//       co mají vlevo zeď nebo označené pole,
//       zůstanou stát.Ostatní se posunou.Kritická podmínka : Pokud robot opustí cílové pole,
//       pole se označí a už na něj nikdo nemůže.To je většinou konečná pro danou cestu
//           .Cíl : Všech $K$ robotů stojí na $K$ cílových
//                  polích(libovolné párování)
//           .Výstup : Všechny nejkratší cesty seřazené lexikograficky.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROBOTS 50

int M, N, K;
int target_r[MAX_ROBOTS], target_s[MAX_ROBOTS];
int start_r[MAX_ROBOTS], start_s[MAX_ROBOTS];

int dr[] = {0, -1, 0, 1}; // L=0, H=1, P=2, D=3
int dc[] = {-1, 0, 1, 0};
char dchar[] = {'L', 'H', 'P', 'D'};

// Kontrola, zda jsou roboti na cílových polích (v libovolném pořadí)
int is_goal(int* r, int* s) {
    int occupied[MAX_ROBOTS] = {0};
    int found_count = 0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            if (!occupied[j] && r[i] == target_r[j] && s[i] == target_s[j]) {
                occupied[j] = 1;
                found_count++;
                break;
            }
        }
    }
    return found_count == K;
}

// Rekurzivní hledání cest
int find_paths(int* curr_r, int* curr_s, unsigned long long mask, int dist, int limit, char* path) {
    if (dist == limit) {
        if (is_goal(curr_r, curr_s)) {
            printf("%s\n", path);
            return 1;
        }
        return 0;
    }

    int success = 0;
    for (int d = 0; d < 4; d++) {
        int next_r[MAX_ROBOTS], next_s[MAX_ROBOTS];
        unsigned long long next_mask = mask;
        int moved_any = 0;

        // Pohyb všech robotů najednou
        for (int i = 0; i < K; i++) {
            int nr = curr_r[i] + dr[d];
            int nc = curr_s[i] + dc[d];

            // Pravidla: v mřížce, není v masce a nestojí tam jiný robot
            if (nr >= 0 && nr < M && nc >= 0 && nc < N && !(mask & (1ULL << (nr * N + nc)))) {
                int collision = 0;
                for (int j = 0; j < K; j++) {
                    if (nr == curr_r[j] && nc == curr_s[j]) {
                        collision = 1;
                        break;
                    }
                }
                if (!collision) {
                    next_r[i] = nr;
                    next_s[i] = nc;
                    next_mask |= (1ULL << (nr * N + nc));
                    moved_any = 1;
                    continue;
                }
            }
            next_r[i] = curr_r[i];
            next_s[i] = curr_s[i];
        }

        if (moved_any) {
            path[dist] = dchar[d];
            path[dist + 1] = '\0';
            if (find_paths(next_r, next_s, next_mask, dist + 1, limit, path)) {
                success = 1;
            }
        }
    }
    return success;
}

int main() {
    // Načtení M, N, K
    if (scanf("%d %d %d", &M, &N, &K) != 3)
        return 0;

    unsigned long long initial_mask = 0;
    for (int i = 0; i < K; i++) {
        scanf("%d %d", &start_r[i], &start_s[i]);
        initial_mask |= (1ULL << (start_r[i] * N + start_s[i]));
    }
    for (int i = 0; i < K; i++) {
        scanf("%d %d", &target_r[i], &target_s[i]);
    }

    char path_buffer[101];
    int found = 0;

    // Iterativní prohlubování: hledáme nejkratší délku od 1 do 20 kroků
    for (int limit = 1; limit <= 20; limit++) {
        if (find_paths(start_r, start_s, initial_mask, 0, limit, path_buffer)) {
            found = 1;
            break; // Našli jsme nejkratší délku, vypíšeme všechny a končíme
        }
    }

    if (!found)
        printf("0\n");

    return 0;
}
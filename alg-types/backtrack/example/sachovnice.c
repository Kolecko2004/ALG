// Algoritmus a strategieReprezentace
//     : Šachovnici uložíme do matice.Budeme si pamatovat pouze souřadnice polí,
//     kde je 1(regulérní pole),
//     abychom nemuseli v rekurzi procházet prázdná místa
//         .Backtracking : Budeme procházet pole jedno po druhém a v každém kroku se rozhodneme
//     : Položit dámu(pokud ji nic neohrožuje)
//         .Nepoložit dámu
//         .Line of Sight(Viditelnost)
//     : Funkce pro kontrolu ohrožení musí v daném
//       směru(řádek, sloupec, diagonály) „narazit“ na první překážku.Pokud narazí na díru(0),
// směr je pro danou dámu bezpečný.Pokud narazí na jinou dámu, je to konflikt.Ořezávání(Pruning)
//     : Aby rekurze neprobíhala do nekonečna u šachovnice $32 \times 32 $,
// použijeme jednoduché ořezávání : pokud(aktuální_počet + zbývající_počet_polí <= max_dosud),
//                                  rekurzi ukončíme.

#include <stdio.h>
#include <stdlib.h>

#define MAX 32

int H, W;
int grid[MAX][MAX];
int queens[MAX][MAX];
int max_queens = 0;

typedef struct {
    int r, c;
} Point;

Point valid_cells[MAX * MAX];
int total_valid = 0;

// Funkce zkontroluje, zda dáma na pozici (r, c) není ohrožena jinou dámou.
// Díky "line of sight" pravidlu díra (0) blokuje ohrožení.
int can_place(int r, int c) {
    // Definice 8 směrů (řádky, sloupce, diagonály)
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    for (int i = 0; i < 8; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        // Postupujeme v daném směru, dokud nevyjedeme z šachovnice
        while (nr >= 0 && nr < H && nc >= 0 && nc < W) {
            // Pokud narazíme na chybějící pole, výhled je zablokován -> tento směr je bezpečný
            if (grid[nr][nc] == 0)
                break;

            // Pokud narazíme na jinou dámu bez mezilehlé díry -> konflikt
            if (queens[nr][nc])
                return 0;

            nr += dr[i];
            nc += dc[i];
        }
    }
    return 1;
}

// Rekurzivní funkce s backtrackingem
void solve(int cell_idx, int current_count) {
    // Aktualizace maxima
    if (current_count > max_queens) {
        max_queens = current_count;
    }

    // Pokud už zbývající pole nestačí k překonání rekordu, ořízneme větev
    if (current_count + (total_valid - cell_idx) <= max_queens) {
        return;
    }

    // Prošli jsme všechna pole
    if (cell_idx == total_valid) {
        return;
    }

    int r = valid_cells[cell_idx].r;
    int c = valid_cells[cell_idx].c;

    // Možnost 1: Zkusíme položit dámu
    if (can_place(r, c)) {
        queens[r][c] = 1;
        solve(cell_idx + 1, current_count + 1);
        queens[r][c] = 0; // Backtrack
    }

    // Možnost 2: Nepoložíme dámu a jdeme dál
    solve(cell_idx + 1, current_count);
}

int main() {
    if (scanf("%d %d", &H, &W) != 2)
        return 0;

    char line[MAX + 1];
    for (int i = 0; i < H; i++) {
        scanf("%s", line);
        for (int j = 0; j < W; j++) {
            grid[i][j] = line[j] - '0';
            if (grid[i][j] == 1) {
                valid_cells[total_valid].r = i;
                valid_cells[total_valid].c = j;
                total_valid++;
            }
        }
    }

    // Spuštění rekurze
    solve(0, 0);

    printf("%d\n", max_queens);

    return 0;
}
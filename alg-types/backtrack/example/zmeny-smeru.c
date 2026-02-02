// Shrnutí zadání "Změny směru" Pohyb
//     : Robot jde z $[0, 0] $ do $[M - 1, N - 1] $ pouze vpravo nebo dolů.K -
//     omezení : Cesta je platná pouze tehdy,
//     pokud robot změní směr(z doprava na dolů nebo obráceně) přesně $K$ - krát.Výpis robota(Pravidla)
//     : První a poslední pole se ignorují.Pokud robot projde polem
//       rovně(např.doprava $\to$ pole $\to$ doprava),
// vypíše znak pole 1x.Pokud v poli zahne(např.doprava $\to$ pole $\to$ dolů), vypíše znak pole 2x.Cíl
//     : Najít řetězce,
//       které se vyskytují nejčastěji.Pokud je jich víc se stejnou(maximální) četností,
//       vypsat je abecedně
//               .Klíčové body řešení : Backtracking
//     : Používáme rekurzi k prozkoumání všech cest
//               .Proměnná current_len a její snižování po návratu z
//       rekurze(current_len -= added) je klasický backtracking.K
//           - kontrola : V rekurzi si neseme k_left.Pokud zahne,
//       odečteme 1. Pokud dojdeme do cíle a k_left != 0,
//       cesta je neplatná.Efektivita : Limit 110 000 cest je pro moderní počítač malý,
//       takže qsort na konci je nejjednodušší způsob,
//       jak seskupit stejné řetězce k sobě a spočítat jejich výskyty.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 100
#define MAX_PATHS 110005

// Globální proměnné pro snadný přístup v rekurzi
char grid[MAX_DIM][MAX_DIM];
int M, N, K;

// Pole pro uložení všech platných výsledných řetězců
char *results[MAX_PATHS];
int results_count = 0;

// Pomocný buffer pro aktuálně generovaný řetězec
// Maximální délka je (M+N)*2, protože znaky se mohou zdvojovat
char current_path[400];
int current_len = 0;

/**
 * Rekurzivní funkce pro hledání cest
 * @param r, s      - aktuální souřadnice (řádek, sloupec)
 * @param k_left    - kolik změn směru nám ještě zbývá provést
 * @param last_dir  - směr, kterým jsme do aktuálního pole PŘIŠLI (0=start, 1=vpravo, 2=dolů)
 */
void find_paths(int r, int s, int k_left, int last_dir) {
    // BÁZOVÝ PŘÍPAD: Dosáhli jsme pravého dolního rohu
    if (r == M - 1 && s == N - 1) {
        // Cesta je platná pouze tehdy, pokud jsme vyčerpali přesně K změn směru
        if (k_left == 0) {
            current_path[current_len] = '\0';
            results[results_count++] = strdup(current_path); // Uložíme kopii řetězce
        }
        return;
    }

    // MOŽNOST A: Pohyb DOPRAVA
    if (s + 1 < N) {
        int next_dir = 1;
        int next_k = k_left;
        int added = 0;

        // Pravidla pro zápis znaků (vynecháváme start [0,0])
        if (last_dir != 0) {
            if (last_dir == next_dir) {
                // Přišli jsme zleva a jdeme doprava -> rovně -> 1 znak
                current_path[current_len++] = grid[r][s];
                added = 1;
            } else {
                // Přišli jsme shora a jdeme doprava -> ZMĚNA SMĚRU -> 2 znaky
                current_path[current_len++] = grid[r][s];
                current_path[current_len++] = grid[r][s];
                added = 2;
                next_k--; // Odečteme změnu směru
            }
        }

        // Rekurze pokračuje, jen pokud jsme nepřekročili limit změn K
        if (next_k >= 0) find_paths(r, s + 1, next_k, next_dir);
        
        // BACKTRACKING: Vrátíme délku řetězce zpět pro další větve rekurze
        current_len -= added;
    }

    // MOŽNOST B: Pohyb DOLŮ
    if (r + 1 < M) {
        int next_dir = 2;
        int next_k = k_left;
        int added = 0;

        if (last_dir != 0) {
            if (last_dir == next_dir) {
                // Přišli jsme shora a jdeme dolů -> rovně -> 1 znak
                current_path[current_len++] = grid[r][s];
                added = 1;
            } else {
                // Přišli jsme zleva a jdeme dolů -> ZMĚNA SMĚRU -> 2 znaky
                current_path[current_len++] = grid[r][s];
                current_path[current_len++] = grid[r][s];
                added = 2;
                next_k--; // Odečteme změnu směru
            }
        }

        if (next_k >= 0) find_paths(r + 1, s, next_k, next_dir);
        
        // BACKTRACKING: Vrátíme délku řetězce zpět
        current_len -= added;
    }
}

// Komparátor pro qsort: seřadí řetězce abecedně
int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    // Načtení rozměrů a počtu změn směru
    if (scanf("%d %d %d", &M, &N, &K) != 3) return 0;

    // Načtení mřížky znaků
    for (int i = 0; i < M; i++) {
        scanf("%s", grid[i]);
    }

    // Spuštění hledání z levého horního rohu [0,0]
    // Počáteční směr je 0 (robot zatím nikam nešel)
    find_paths(0, 0, K, 0);

    // Abychom našli nejčastější řetězce, nejdříve je všechny seřadíme.
    // Stejné řetězce se tak dostanou vedle sebe.
    qsort(results, results_count, sizeof(char *), compare_strings);

    // PRVNÍ PRŮCHOD: Zjištění maximální četnosti
    int max_freq = 0;
    int current_freq = 0;
    for (int i = 0; i < results_count; i++) {
        current_freq++;
        // Pokud je další řetězec jiný nebo jsme na konci pole
        if (i == results_count - 1 || strcmp(results[i], results[i + 1]) != 0) {
            if (current_freq > max_freq) {
                max_freq = current_freq;
            }
            current_freq = 0;
        }
    }

    // DRUHÝ PRŮCHOD: Výpis všech řetězců, které mají tuto max_freq
    current_freq = 0;
    for (int i = 0; i < results_count; i++) {
        current_freq++;
        if (i == results_count - 1 || strcmp(results[i], results[i + 1]) != 0) {
            if (current_freq == max_freq) {
                printf("%s\n", results[i]);
            }
            current_freq = 0;
        }
    }

    // Uvolnění dynamicky alokované paměti
    for (int i = 0; i < results_count; i++) {
        free(results[i]);
    }

    return 0;
}
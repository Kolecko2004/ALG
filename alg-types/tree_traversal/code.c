// Hledani nejakeho prumeru nebo co ve strome
// vytvareni stromu a nasledne prohledavani aby se nasla nejaka ta prumerna hodnota

#include <stdio.h>
#include <stdlib.h>

#define MAX_D 100
#define MAIN_MOD 4

/* Definice struktury uzlu stromu. 
   Používáme 'struct Node*', aby překladač věděl, že se odkazujeme na stejný typ. */
typedef struct Node {
    struct Node* left;
    struct Node* right;
    int value;
} Node;

/* Globální parametry pro generování stromu */
int max_depth, A, C, mod, first_val;

/* Globální čítač listů v celém stromě pro efektivní alokaci paměti */
long long leaf_num = 0;

/* Načtení vstupní pětice čísel (A, C, M, Z, D) */
void load_input() { 
    fscanf(stdin, "%d %d %d %d %d", &A, &C, &mod, &first_val, &max_depth); 
}

/* Pomocná funkce pro třídění pole celých čísel pomocí qsort */
int compare_ints(const void* a, const void* b) { 
    return (*(int*) a - *(int*) b); 
}

/* Funkce vrací 1, pokud uzel není list (má alespoň jednoho potomka) */
int is_internal(Node* n) { 
    return (n->left != NULL || n->right != NULL); 
}

/* Výpočet Redukovaného Levého Mediánu (RLM) */
int get_rlm(int* leaves, int count) {
    if (count == 0) return 0;

    // 1. Seřazení listů (nutné pro medián i odstranění duplicit)
    qsort(leaves, count, sizeof(int), compare_ints);

    // 2. Vytvoření množiny H (odstranění duplicit ze souboru klíčů)
    int h[leaf_num]; 
    int h_size = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0 || leaves[i] != leaves[i - 1]) {
            h[h_size++] = leaves[i];
        }
    }

    // 3. Výběr levého mediánu z množiny H podle pravidel v zadání
    if (h_size % 2 != 0) {
        // Lichý počet: právě (|H|-1)/2 prvků větších
        return h[(h_size - 1) / 2];
    } else {
        // Sudý počet: právě |H|/2 prvků větších
        return h[(h_size / 2) - 1];
    }
}

/* Rekurzivní funkce, která projde podstrom uzlu n a zapíše klíče listů do pole */
void collect_leaves(Node* n, int* leaf_values, int* count) {
    if (n == NULL) return;

    // Uzel je list, pokud nemá levou ani pravou větev
    if (n->left == NULL && n->right == NULL) {
        leaf_values[*count] = n->value;
        (*count)++;
        return;
    }

    collect_leaves(n->left, leaf_values, count);
    collect_leaves(n->right, leaf_values, count);
}

/* Funkce pro procházení všech vnitřních uzlů a hledání min/max potenciálu */
void calculate(Node* node, long long* min, long long* max) {
    if (node == NULL) return;

    if (is_internal(node)) {
        // Alokujeme paměť na haldě podle počtu listů v celém stromě (ochrana před Segfault)
        int* leaves = malloc(leaf_num * sizeof(int));
        if (leaves == NULL) return;

        int count = 0;
        collect_leaves(node, leaves, &count); // Najdeme listy v aktuálním podstromu
        
        int rlm = get_rlm(leaves, count); // Spočítáme medián listů
        
        // Potenciál uzlu x = K(x) * RLM(x)
        long long potential = (long long)node->value * rlm;

        // Aktualizujeme minimum a maximum (pokud je hodnota -1, jde o první zápis)
        if (*min == -1 || potential < *min) *min = potential;
        if (*max == -1 || potential > *max) *max = potential;

        free(leaves); // Uvolnění paměti po výpočtu v daném uzlu
    }

    // Rekurzivní volání pro potomky
    calculate(node->left, min, max);
    calculate(node->right, min, max);
}

/* Rekurzivní stavba stromu na základě pravidel 0-7 */
Node* build_tree(int val, int depth) {
    // Pravidlo 2: Maximální hloubka stromu je D
    if (depth > max_depth) { 
        return NULL;
    }

    Node* node = malloc(sizeof(Node));
    node->value = val;
    node->left = NULL;
    node->right = NULL;

    // O větvení rozhoduje hodnota klíče modulo 4 (Pravidla 3 a 4)
    int branch_rule = val % MAIN_MOD;

    // Pravidlo 3 a 5: Levý potomek s klíčem F(K(x)+1)
    if (branch_rule == 1 || branch_rule == 3) {
        int left_val = (1LL * A * (val + 1) + C) % mod; 
        node->left = build_tree(left_val, depth + 1);
    }

    // Pravidlo 4 a 6: Pravý potomek s klíčem F(K(x)+2)
    if (branch_rule == 2 || branch_rule == 3) {
        int right_val = (1LL * A * (val + 2) + C) % mod;
        node->right = build_tree(right_val, depth + 1);
    }

    // Pokud je uzel list, zvýšíme globální počítadlo
    if (node->right == NULL && node->left == NULL) {
        leaf_num++;
    }

    return node;
}

int main() {
    load_input();

    // Pravidlo 7: Kořen k má klíč K(k) = Z
    Node* root = build_tree(first_val, 0);

    // Inicializace min/max na -1 pro indikaci dosud nenalezených vnitřních uzlů
    long long min_p = -1;
    long long max_p = -1;

    calculate(root, &min_p, &max_p);

    // Pokud strom obsahuje vnitřní uzly, vypíšeme min a max potenciál
    if (max_p != -1) {
        fprintf(stdout, "%lld %lld\n", min_p, max_p);
    }

    return 0;
}
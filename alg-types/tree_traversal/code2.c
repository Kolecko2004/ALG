#include <stdio.h>
#include <stdlib.h>

#define MAX_D 100
#define MAIN_MOD 4

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int value;
    double path_right;
    double path_left;
} Node;

/* Struktura pro předávání výsledků z podstromů nahoru */
typedef struct {
    long long total_dist; // Součet vzdáleností ke všem listům v podstromu
    int leaf_count;       // Počet listů v podstromu
} Result;

int max_depth, A, C, mod, first_val;

void load_input() { 
    if (scanf("%d %d %d %d %d", &A, &C, &mod, &first_val, &max_depth) != 5) return; 
}

/* Hlavní výpočetní funkce: Prochází strom a počítá komunikační index (CI) */
Result process_node(Node* n, double* min_ci, double* max_ci) {
    Result res = {0, 0};
    if (n == NULL) return res;

    // Pokud je uzel list (Pravidlo: vzdálenost k sobě samému je 0)
    if (n->left == NULL && n->right == NULL) {
        res.leaf_count = 1;
        res.total_dist = 0;
        return res;
    }

    // Rekurzivní získání dat od dětí
    Result left_res = process_node(n->left, min_ci, max_ci);
    Result right_res = process_node(n->right, min_ci, max_ci);

    // Počet listů pod aktuálním uzlem je součet listů jeho dětí
    res.leaf_count = left_res.leaf_count + right_res.leaf_count;
    
    // Nový součet vzdáleností: 
    // Pro každý list vlevo se vzdálenost zvětší o délku hrany k levému dítěti (path_left)
    if (n->left) 
        res.total_dist += left_res.total_dist + (long long)left_res.leaf_count * n->path_left;
    if (n->right) 
        res.total_dist += right_res.total_dist + (long long)right_res.leaf_count * n->path_right;

    // Výpočet CI (Komunikační Index) pro aktuální vnitřní uzel
    if (res.leaf_count > 0) {
        double current_ci = (double)res.total_dist / res.leaf_count;
        if (*min_ci == -1 || current_ci < *min_ci) *min_ci = current_ci;
        if (*max_ci == -1 || current_ci > *max_ci) *max_ci = current_ci;
    }

    return res;
}

/* Stavba stromu s výpočtem vah hran */
Node* build_tree(int val, int depth) {
    if (depth >= max_depth) return NULL; // Oprava: hloubka D je limit hran

    Node* node = malloc(sizeof(Node));
    if (!node) return NULL;
    node->value = val;
    node->left = NULL;
    node->right = NULL;
    node->path_left = 0;
    node->path_right = 0;

    int branch_rule = val % MAIN_MOD;

    if (branch_rule == 1 || branch_rule == 3) {
        int left_val = (int)((1LL * A * (val + 1) + C) % mod);
        // Vzdálenost: (K(x) + K(y)) mod 10 + 1
        node->path_left = ((val + left_val) % 10) + 1;
        node->left = build_tree(left_val, depth + 1);
    }

    if (branch_rule == 2 || branch_rule == 3) {
        int right_val = (int)((1LL * A * (val + 2) + C) % mod);
        node->path_right = ((val + right_val) % 10) + 1;
        node->right = build_tree(right_val, depth + 1);
    }

    return node;
}

int main() {
    load_input();

    Node* root = build_tree(first_val, 0);

    double min_p = -1;
    double max_p = -1;

    process_node(root, &min_p, &max_p);

    if (max_p != -1) {
        // Výpis zaokrouhlený dolů (přetypováním na long long)
        printf("%lld %lld\n", (long long)min_p, (long long)max_p);
    }

    return 0;
}
// Shrnutí postupuRekurzivní generování
//     : Funkce build_tree vytváří strom shora dolů podle matematických pravidel.Tím
//           získáme topologii.Post -
//     order merge
//     : Funkce process_potential jde
//       "zespodu".Listy prostě vrátí svou hodnotu.Vnitřní uzel počká na seznamy od svých dětí,
//     slije je do jednoho seřazeného unikátního pole a z něj "vypíchne" medián.Matematika indexu
//     : Zadání mediánu říká : Liché $ |
//         H |=
//     5 \to$ chceme $(5 - 1) /
//     2 = 2 $ prvky větší než $x$.To je prvek na indexu 2.Sudé $ | H |=
//     6 \to$ chceme $6 /
//     2 = 3 $ prvky větší než $x$.To je prvek na indexu 2.V obou případech v C(celočíselné dělení)
//             funguje vzorec $idx = (count - 1) / 2 $.

//                                                 2. Strategie řešeníVzhledem k tomu,
//     že hloubka stromu je malá($D \le 100 $), ale počet uzlů může být až $150\, 000 $,
//     musíme efektivně předávat seznamy listů od dětí k rodičům.Ideální je použít slučování
//     seřazených polí(podobně jako v Merge Sortu), čímž udržíme listy seřazené a zároveň snadno
//     odstraníme duplicity.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

// Struktura pro uzel stromu
typedef struct Node {
    ll key;
    struct Node *left, *right;
    int is_leaf;
} Node;

ll A, C, M, Z;
int D;
ll min_pot = -1, max_pot = -1;

// Generátor klíčů podle zadání
ll F(ll n) { return (A * n + C) % M; }

// Rekurzivní stavba stromu
Node* build_tree(ll key, int depth) {
    if (depth > D)
        return NULL;

    Node* node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = 1;

    // Podmínky pro levého potomka: mod 4 je 1 nebo 3
    if ((key % 4 == 1 || key % 4 == 3) && depth < D) {
        node->left = build_tree(F(key + 1), depth + 1);
        if (node->left)
            node->is_leaf = 0;
    }
    // Podmínky pro pravého potomka: mod 4 je 2 nebo 3
    if ((key % 4 == 2 || key % 4 == 3) && depth < D) {
        node->right = build_tree(F(key + 2), depth + 1);
        if (node->right)
            node->is_leaf = 0;
    }

    return node;
}

// Struktura pro seznam unikátních listů
typedef struct {
    ll* values;
    int count;
} LeafList;

// Funkce pro sloučení dvou seřazených seznamů unikátních hodnot
LeafList merge_leaf_lists(LeafList a, LeafList b) {
    LeafList res;
    // Maximální možná velikost je součet obou, skutečná bude po odstranění duplicit menší
    res.values = (ll*) malloc(sizeof(ll) * (a.count + b.count));
    int i = 0, j = 0, k = 0;

    // OPRAVA: Správná podmínka cyklu
    while (i < a.count || j < b.count) {
        ll val;
        if (i < a.count && (j == b.count || a.values[i] < b.values[j])) {
            val = a.values[i++];
        } else if (j < b.count && (i == a.count || b.values[j] < a.values[i])) {
            val = b.values[j++];
        } else {
            // Hodnoty jsou stejné - vezmeme jednu a posuneme oba indexy (deduplikace)
            val = a.values[i++];
            j++;
        }

        // Přidáme hodnotu do výsledku, pokud tam ještě není (pro jistotu)
        if (k == 0 || res.values[k - 1] != val) {
            res.values[k++] = val;
        }
    }
    res.count = k;
    return res;
}

// Hlavní průchod pro výpočet potenciálu
LeafList process_potential(Node* node) {
    if (node->is_leaf) {
        LeafList res;
        res.count = 1;
        res.values = (ll*) malloc(sizeof(ll));
        res.values[0] = node->key;
        return res;
    }

    LeafList left_list = {NULL, 0};
    LeafList right_list = {NULL, 0};

    if (node->left)
        left_list = process_potential(node->left);
    if (node->right)
        right_list = process_potential(node->right);

    LeafList merged = merge_leaf_lists(left_list, right_list);

    // Výpočet RLM: index (count - 1) / 2
    ll rlm = merged.values[(merged.count - 1) / 2];
    ll potential = node->key * rlm;

    // Update min/max (vnitřní uzly)
    if (min_pot == -1 || potential < min_pot)
        min_pot = potential;
    if (max_pot == -1 || potential > max_pot)
        max_pot = potential;

    // Uvolnění dočasných seznamů
    if (left_list.values)
        free(left_list.values);
    if (right_list.values)
        free(right_list.values);

    return merged;
}

int main() {
    if (scanf("%lld %lld %lld %lld %d", &A, &C, &M, &Z, &D) != 5)
        return 0;

    Node* root = build_tree(Z, 0);

    LeafList final_list = process_potential(root);

    printf("%lld %lld\n", min_pot, max_pot);

    // Čištění (pro reálnou soutěž volitelné, OS si to pořeší)
    free(final_list.values);

    return 0;
}
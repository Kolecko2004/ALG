// Strategie řešeníGenerování stromů
//     : Postavíme stromy $T_1$ a $T_2$ podle zadaných pravidel.$K \pmod 4 =
//     0 $ : Žádné dítě.$K \pmod 4 = 1 $ : Pouze levé.$K \pmod 4 = 2 $ : Pouze pravé.$K \pmod 4 =
//         3 $ : Obě děti(v zadání je u bodu 'd' pravděpodobně překlep, logicky následuje hodnota 3)
//                   .Identifikace
//                   tvarů(Hashing)
//     : Abychom mohli rychle porovnat,
//                                                                       zda mají dva podstromy
//                                                                       stejný
//                                                                           tvar,
//                                                                       přiřadíme každému
//                                                                       unikátnímu
//                                                                           tvaru ID.Tvar listu má
//                                                                               ID =
//                                                                           1.Tvar vnitřního uzlu
//                                                                           je
//                                                                               dán
//                                                                               dvojicí(
//                                                                                   ID_levého_dítěte,
//                                                                                   ID_pravého_dítěte)
//                                                                                   .Použijeme
//                                                                                       hashovací
//                                                                                           tabulku,
//                                                                       která mapuje tyto dvojice
//                                                                       na
//                                                                               unikátní celá čísla
//                                                                                   .Výpočet
//                                                                                   parametrů
//                                                                                       $S$ a $H$
//     : Pro každý uzel v obou stromech spočítáme : $S$(počet uzlů)
//     : $1 + S_{vlevo} +
//                                                                           S_{vpravo} $.$H$(výška)
//     : $1 + \max(H_{vlevo}, H_{vpravo}) $(pro list je $H = 0 $)
//                                                                               .Hledání maxima
//     : Uložíme si všechny unikátní tvary z prvního stromu $T_1$ spolu s jejich
//       $(S, H) $.Projdeme druhý strom $T_2$ a pokud najdeme tvar,
//                                                                       který byl v $T_1$,
//                                                                       porovnáme jeho $(S, H)
// $ s aktuálním maximem podle lexikografického pravidla(priorita má $S$, při rovnosti $H$)
.

#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 300000
#define HASH_SIZE 1000003

    typedef long long ll;

int A, C, M, D;

typedef struct Node {
    int s, h, shape_id;
    struct Node *l, *r;
} Node;

// Hashovací tabulka pro mapování (left_id, right_id) -> unique_shape_id
typedef struct {
    int l_id, r_id, shape_id;
} ShapeEntry;

ShapeEntry hash_table[HASH_SIZE];
int next_shape_id = 2; // 0 = empty, 1 = leaf

int get_shape_id(int l_id, int r_id) {
    if (l_id == 0 && r_id == 0)
        return 1; // List

    ll hash = ((ll) l_id * 31337 + r_id) % HASH_SIZE;
    while (hash_table[hash].shape_id != 0) {
        if (hash_table[hash].l_id == l_id && hash_table[hash].r_id == r_id)
            return hash_table[hash].shape_id;
        hash = (hash + 1) % HASH_SIZE;
    }

    hash_table[hash].l_id = l_id;
    hash_table[hash].r_id = r_id;
    hash_table[hash].shape_id = next_shape_id++;
    return hash_table[hash].shape_id;
}

// Rekurzivní stavba a analýza stromu
Node* build_tree(int K, int depth) {
    if (depth > D)
        return NULL;

    Node* n = (Node*) malloc(sizeof(Node));
    int type = K % 4;

    n->l = (type == 1 || type == 3) ? build_tree((1LL * A * (K + 1) + C) % M, depth + 1) : NULL;
    n->r = (type == 2 || type == 3) ? build_tree((1LL * A * (K + 2) + C) % M, depth + 1) : NULL;

    // Počet uzlů S
    n->s = 1 + (n->l ? n->l->s : 0) + (n->r ? n->r->s : 0);

    // Výška H (počet hran)
    int hl = (n->l ? n->l->h + 1 : 0);
    int hr = (n->r ? n->r->h + 1 : 0);
    n->h = (hl > hr ? hl : hr);
    if (!n->l && !n->r)
        n->h = 0;

    // Unikátní ID tvaru
    int lid = (n->l ? n->l->shape_id : 0);
    int rid = (n->r ? n->r->shape_id : 0);
    n->shape_id = get_shape_id(lid, rid);

    return n;
}

// Mapování existujících tvarů v T1
char shapes_in_t1[MAX_NODES * 2];
int s_val[MAX_NODES * 2], h_val[MAX_NODES * 2];

void collect_shapes(Node* n) {
    if (!n)
        return;
    shapes_in_t1[n->shape_id] = 1;
    s_val[n->shape_id] = n->s;
    h_val[n->shape_id] = n->h;
    collect_shapes(n->l);
    collect_shapes(n->r);
}

int max_s = 0, max_h = 0;

void find_max_match(Node* n) {
    if (!n)
        return;
    if (shapes_in_t1[n->shape_id]) {
        if (n->s > max_s || (n->s == max_s && n->h > max_h)) {
            max_s = n->s;
            max_h = n->h;
        }
    }
    find_max_match(n->l);
    find_max_match(n->r);
}

int main() {
    int Z1, Z2;
    if (scanf("%d %d %d %d %d %d", &Z1, &Z2, &A, &C, &M, &D) != 6)
        return 0;

    Node* T1 = build_tree(Z1, 0);
    Node* T2 = build_tree(Z2, 0);

    collect_shapes(T1);
    find_max_match(T2);

    printf("%d %d\n", max_s, max_h);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// #define MAX_NODES 300005

// typedef struct Node {
//     int s, h, shape_id;
//     int tree_id; // 1 pro T1, 2 pro T2
//     struct Node *l, *r;
// } Node;

// typedef struct {
//     int l_id, r_id;
//     Node* node;
// } ShapeTuple;

// int A, C, M, D;

// // Generování stromu
// Node* build_tree(int K, int depth, int tree_id) {
//     if (depth > D) return NULL;

//     Node* n = (Node*)malloc(sizeof(Node));
//     n->tree_id = tree_id;
//     int type = K % 4;

//     // Logika potomků: 0:nic, 1:L, 2:P, 3:oba (v zadání opraveno na 3)
//     n->l = (type == 1 || type == 3) ? build_tree((1LL * A * (K + 1) + C) % M, depth + 1, tree_id)
//     : NULL; n->r = (type == 2 || type == 3) ? build_tree((1LL * A * (K + 2) + C) % M, depth + 1,
//     tree_id) : NULL;

//     n->s = 1 + (n->l ? n->l->s : 0) + (n->r ? n->r->s : 0);
//     int hl = (n->l ? n->l->h + 1 : 0);
//     int hr = (n->r ? n->r->h + 1 : 0);
//     n->h = (hl > hr ? hl : hr);
//     if (!n->l && !n->r) n->h = 0;

//     return n;
// }

// // Pomocná funkce pro qsort
// int compare_shapes(const void* a, const void* b) {
//     ShapeTuple* sa = (ShapeTuple*)a;
//     ShapeTuple* sb = (ShapeTuple*)b;
//     if (sa->l_id != sb->l_id) return sa->l_id - sb->l_id;
//     return sa->r_id - sb->r_id;
// }

// Node* all_nodes[MAX_NODES * 2];
// int node_count = 0;

// void collect_all(Node* n) {
//     if (!n) return;
//     all_nodes[node_count++] = n;
//     collect_all(n->l);
//     collect_all(n->r);
// }

// // Pole pro uložení informace, zda je tvar v T1
// char shape_in_t1[MAX_NODES * 2];
// int shape_s[MAX_NODES * 2];
// int shape_h[MAX_NODES * 2];

// int main() {
//     int Z1, Z2;
//     if (scanf("%d %d %d %d %d %d", &Z1, &Z2, &A, &C, &M, &D) != 6) return 0;

//     Node* T1 = build_tree(Z1, 0, 1);
//     Node* T2 = build_tree(Z2, 0, 2);

//     collect_all(T1);
//     collect_all(T2);

//     // Seřadíme všechny uzly podle jejich výšky H (bottom-up zpracování)
//     // Pro jednoduchost použijeme seskupování podle H od 0 do D
//     int next_id = 2; // 1 je vyhrazeno pro listy
//     ShapeTuple* tuples = malloc(sizeof(ShapeTuple) * node_count);

//     for (int h = 0; h <= D; h++) {
//         int count_at_h = 0;
//         for (int i = 0; i < node_count; i++) {
//             if (all_nodes[i]->h == h) {
//                 if (h == 0) {
//                     all_nodes[i]->shape_id = 1;
//                 } else {
//                     tuples[count_at_h].l_id = all_nodes[i]->l ? all_nodes[i]->l->shape_id : 0;
//                     tuples[count_at_h].r_id = all_nodes[i]->r ? all_nodes[i]->r->shape_id : 0;
//                     tuples[count_at_h].node = all_nodes[i];
//                     count_at_h++;
//                 }
//             }
//         }

//         if (h > 0 && count_at_h > 0) {
//             qsort(tuples, count_at_h, sizeof(ShapeTuple), compare_shapes);
//             for (int i = 0; i < count_at_h; i++) {
//                 if (i > 0 && tuples[i].l_id == tuples[i-1].l_id && tuples[i].r_id ==
//                 tuples[i-1].r_id) {
//                     tuples[i].node->shape_id = tuples[i-1].node->shape_id;
//                 } else {
//                     tuples[i].node->shape_id = next_id++;
//                 }
//             }
//         }
//     }

//     // Teď jen najdeme největší (S, H) index, který je v obou stromech
//     for (int i = 0; i < node_count; i++) {
//         int id = all_nodes[i]->shape_id;
//         if (all_nodes[i]->tree_id == 1) {
//             shape_in_t1[id] = 1;
//             shape_s[id] = all_nodes[i]->s;
//             shape_h[id] = all_nodes[i]->h;
//         }
//     }

//     int res_s = 0, res_h = 0;
//     for (int i = 0; i < node_count; i++) {
//         if (all_nodes[i]->tree_id == 2) {
//             int id = all_nodes[i]->shape_id;
//             if (shape_in_t1[id]) {
//                 if (shape_s[id] > res_s || (shape_s[id] == res_s && shape_h[id] > res_h)) {
//                     res_s = shape_s[id];
//                     res_h = shape_h[id];
//                 }
//             }
//         }
//     }

//     printf("%d %d\n", res_s, res_h);

//     free(tuples);
//     return 0;
// }
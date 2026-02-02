// Shrnutí úlohy
// Cílem je analyzovat graf, kde uzly jsou čísla v určitém rozsahu a hrana mezi nimi existuje, pokud jejich parametry splňují podmínku s největším společným dělitelem (NSD/GCD).

// Počet komponent: Kolik samostatných "ostrovů" v grafu existuje.

// Maximální poloměr: Pro každou komponentu musíme najít uzel, který je "nejblíže ke všem ostatním" (střed), a zjistit jeho vzdálenost k tomu 
// nejvzdálenějšímu (excentricita). Nás zajímá nejvyšší poloměr napříč všemi komponentami.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Funkce pro výpočet největšího společného dělitele (Eukleidův algoritmus)
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        long long tmp = a;
        a = b;
        b = tmp;
    }
    return a;
}

// Funkce pro výpočet absolutní hodnoty
long long abs_val(long long x) {
    return x < 0 ? -x : x;
}

// Struktura pro frontu v BFS
typedef struct {
    int *data;
    int head;
    int tail;
} Queue;

void enqueue(Queue *q, int val) { q->data[q->tail++] = val; }
int dequeue(Queue *q) { return q->data[q->head++]; }
bool is_empty(Queue *q) { return q->head == q->tail; }

int main() {
    long long N1, N2, D;
    if (scanf("%lld %lld %lld", &N1, &N2, &D) != 3) return 0;

    int num_nodes = (int)(N2 - N1 + 1);
    
    // 1. Sestavení matice sousednosti (pro 1000 uzlů je to cca 1MB, což je OK)
    // adj[i][j] == 1 znamená, že mezi uzly (N1+i) a (N1+j) je hrana
    bool **adj = malloc(num_nodes * sizeof(bool *));
    for (int i = 0; i < num_nodes; i++) {
        adj[i] = calloc(num_nodes, sizeof(bool));
    }

    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            long long a = N1 + i;
            long long b = N1 + j;
            long long diff = abs_val(a - b);
            
            // Podmínka ze zadání
            long long val1 = a + b + diff + 3;
            long long val2 = a * b + diff + 2;
            
            if (gcd(val1, val2) >= D) {
                adj[i][j] = adj[j][i] = true;
            }
        }
    }

    bool *visited = calloc(num_nodes, sizeof(bool));
    int *comp_nodes = malloc(num_nodes * sizeof(int));
    int num_components = 0;
    int max_radius = 0;

    Queue q;
    q.data = malloc(num_nodes * sizeof(int));
    int *dist = malloc(num_nodes * sizeof(int));

    // 2. Procházení komponent
    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i]) {
            num_components++;
            int comp_size = 0;

            // BFS pro nalezení všech uzlů v této komponentě
            q.head = q.tail = 0;
            enqueue(&q, i);
            visited[i] = true;
            while (!is_empty(&q)) {
                int u = dequeue(&q);
                comp_nodes[comp_size++] = u;
                for (int v = 0; v < num_nodes; v++) {
                    if (adj[u][v] && !visited[v]) {
                        visited[v] = true;
                        enqueue(&q, v);
                    }
                }
            }

            // 3. Výpočet poloměru komponenty
            // Poloměr = min(excentricita uzlu) pro všechny uzly v komponentě
            int min_eccentricity = -1;

            for (int k = 0; k < comp_size; k++) {
                int start_node = comp_nodes[k];
                
                // BFS pro zjištění vzdáleností z uzlu start_node
                for (int n = 0; n < num_nodes; n++) dist[n] = -1;
                q.head = q.tail = 0;
                enqueue(&q, start_node);
                dist[start_node] = 0;
                int current_max_dist = 0;

                while (!is_empty(&q)) {
                    int u = dequeue(&q);
                    if (dist[u] > current_max_dist) current_max_dist = dist[u];
                    
                    for (int v = 0; v < num_nodes; v++) {
                        if (adj[u][v] && dist[v] == -1) {
                            dist[v] = dist[u] + 1;
                            enqueue(&q, v);
                        }
                    }
                }
                
                // Excentricita tohoto uzlu je nejdelší z nejkratších cest
                if (min_eccentricity == -1 || current_max_dist < min_eccentricity) {
                    min_eccentricity = current_max_dist;
                }
            }
            
            if (min_eccentricity > max_radius) max_radius = min_eccentricity;
        }
    }

    printf("%d %d\n", num_components, max_radius);

    // Uvolnění paměti
    for (int i = 0; i < num_nodes; i++) free(adj[i]);
    free(adj);
    free(visited);
    free(comp_nodes);
    free(q.data);
    free(dist);

    return 0;
}
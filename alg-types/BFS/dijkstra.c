#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 6 // Počet vrcholů v grafu

// Funkce pro nalezení vrcholu s minimální vzdáleností,
// který ještě nebyl zahrnut do stromu nejkratších cest.
int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// Funkce pro výpis výsledků
void printSolution(int dist[]) {
    printf("Vrchol \t Vzdálenost od zdroje\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

void dijkstra(int graph[V][V], int src) {
    int dist[V];    // Výsledné vzdálenosti dist[i] bude nejkratší cesta ze src do i
    bool sptSet[V]; // true, pokud je vrchol i zahrnut v nejkratší cestě

    // Inicializace: všechny vzdálenosti na nekonečno, sptSet na false
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    // Vzdálenost zdroje od sebe sama je vždy 0
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        // Vybereme vrchol s minimální vzdáleností z dosud nezpracovaných
        int u = minDistance(dist, sptSet);

        // Označíme vrchol jako zpracovaný
        sptSet[u] = true;

        // Aktualizujeme hodnoty dist sousedních vrcholů vybraného vrcholu
        for (int v = 0; v < V; v++) {
            // Aktualizace dist[v] pouze pokud:
            // 1. není v sptSet
            // 2. existuje hrana z u do v
            // 3. celková váha cesty přes u je menší než aktuální dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printSolution(dist);
}

int main() {
    /* Příklad grafu (matice sousednosti)
       0 znamená, že mezi uzly není přímá hrana */
    int graph[V][V] = { { 0, 4, 0, 0, 0, 0 },
                        { 4, 0, 8, 0, 0, 0 },
                        { 0, 8, 0, 7, 0, 4 },
                        { 0, 0, 7, 0, 9, 14 },
                        { 0, 0, 0, 9, 0, 10 },
                        { 0, 0, 4, 14, 10, 0 } };

    dijkstra(graph, 0);

    return 0;
}
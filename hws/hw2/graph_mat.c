#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph_mat.h"

struct mat_graph {
    int num_nodes;
    bool **edges;
};

graph *create_graph(int num_nodes) {
    graph *g = malloc(sizeof(graph));
    if (g == NULL) {
        fprintf(stderr, "ERROR: while alocating memory!\n");
        return 0;
    }

    g -> num_nodes = num_nodes;

    // allocate matrix
    g -> edges = calloc(sizeof(bool*), g -> num_nodes);

    if (g -> edges == NULL) {
        fprintf(stderr, "ERROR: while alocating memory!\n");
        return 0;
    }

    for (int i = 0; i < g -> num_nodes; i++) {
        g -> edges[i] = calloc(sizeof(bool), g -> num_nodes);
        if (g -> edges[i] == NULL) {
            fprintf(stderr, "ERROR: while alocating memory!1\n");
            free_graph(g);
            return 0;
        }
    }

    return g;
}

void free_graph(graph* g) {
    if (g -> edges == NULL) {
        return;
    }

    for (int i = 0; i < g -> num_nodes; i++) {
        if (g -> edges[i] != NULL) {
            free(g -> edges[i]);
        }
    }
    
    free (g-> edges);
}

void print_graph(graph* g) {
    fprintf(stdout, "diagraph {\n");

    for (int from = 0; from < g -> num_nodes; from++)
    {
        for (int to; to < g -> num_nodes; to++)
        {
            if (g -> edges[from][to]) {
                fprintf(stdout, "%d -> %d;\n",  from, to);
            }
        }
        
    }

    fprintf(stdout, "}\n");
    
}

bool add_edge(graph* g, unsigned int from_node, unsigned int to_node) {
    return false;
}

bool has_edge(graph* g, unsigned int from_node, unsigned int to_node) {
    return false;
}
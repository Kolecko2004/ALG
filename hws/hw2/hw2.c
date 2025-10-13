// Homework 2 - agents

#include "graph_mat.h"

int main() {
    graph *g1 = create_graph(5);

    print_graph(g1);

    free_graph(g1);

    return 0;
}
#ifndef GRAPH_MAT_H_
#define GRAPH_MAT_H_

#include <stdbool.h>

typedef struct mat_graph graph;

graph *create_graph(int num_nodes);

void free_graph(graph* g);

void print_graph(graph* g);

bool add_edge(graph* g, unsigned int from_node, unsigned int to_node);

bool has_edge(graph* g, unsigned int from_node, unsigned int to_node);

#endif
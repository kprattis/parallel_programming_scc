#ifndef SEQ_SCC_H
#define SEQ_SCC_H

#include "graph.h"

int scc(FILE* f, int **);

void pred(graph *g, int* frontier, int* nscc);

int push_colors(graph *g);

void trim(graph *g);

#endif

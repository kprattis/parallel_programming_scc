#ifndef CILK_SCC_H
#define CILK_SCC_H

#include "graph.h"

int scc(FILE* f, int **);

void pred(graph *g, int* unique, int* nscc);

int push_colors(graph *g);

int trim(graph *g);

#endif

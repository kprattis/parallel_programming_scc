#ifndef OMP_SCC_H
#define OMP_SCC_H

#include "graph.h"

int scc(FILE* f, int **);

void pred(graph *g, int* frontier, int* nscc);

int push_colors(graph *g);

int trim(graph *g);

#endif
#ifndef CILK_SCC_H
#define CILK_SCC_H

#include <stdlib.h>
#include <stdlib.h>
#include "graph.h"
#include <cilk/cilk.h>

int scc(FILE* f, int **);

void pred(graph *g, int c, int nscc);

int push_colors(graph *g);

void trim(graph *g);

#endif

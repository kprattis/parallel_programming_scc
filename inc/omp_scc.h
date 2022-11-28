#ifndef OMP_SCC_H
#define OMP_SCC_H

#include <stdlib.h>
#include <stdlib.h>
#include "graph.h"
#include "omp.h"

int omp_scc(FILE* f, int **);

void omp_pred(graph *g, int* frontier, int* nscc);

int omp_push_colors(graph *g);

void omp_trim(graph *g);

#endif
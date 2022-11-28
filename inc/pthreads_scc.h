#ifndef PTHREAD_SCC_H
#define PTHREAD_SCC_H

#include <stdlib.h>
#include <stdlib.h>
#include "graph.h"
#include <pthread.h>

#define NTHREADS 12


typedef struct{
	int id;
    int* flag;
    int** unique;
    int *n_unique;
    int **n_scc;
}param;

pthread_attr_t attr;
pthread_mutex_t mut;
pthread_cond_t available_thread;

extern graph* g;

int scc(FILE* f, int **);

void pred(graph *g, int* frontier, int* nscc);

void* push_colors(void*);

void trim(graph *g);

#endif

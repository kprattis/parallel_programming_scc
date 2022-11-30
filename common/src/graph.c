#include "graph.h"
#include "sparse.h"
#include "read_mtx.h"
#include <stdio.h>
#include <stdlib.h>

graph* init_graph(FILE* f){
    graph *g = (graph *) malloc(sizeof(graph));
    g->n_scc = 0;
    	int N, nnz;
    	int *rows, *cols;
	read_mtx(f, &N, &nnz, &rows, &cols);
	g->n = N;
	g->nnz = nnz;
	
	g->is_empty = (g->n == 0);

	g->csr = (sparse *) malloc(sizeof(sparse));
	g->csr->ptr = (int *) calloc(g->n + 1, sizeof(int));
	g->csr->ind = (int *) malloc(g->nnz * sizeof(int));

	g->csc = (sparse *) malloc(sizeof(sparse));
	g->csc->ptr = (int *) calloc(g->n + 1, sizeof(int));
	g->csc->ind = (int *) malloc(g->nnz * sizeof(int));
	
	coo2csc(rows, cols, g->csc, g->n, g->nnz);
	
	csc2csr(g->csc, g->csr, g->n, g->nnz);

	g->removed = (int *) calloc(g->n, sizeof(int));
    g->colors = (int *) malloc(g->n * sizeof(int));
    g->scc = (int * ) malloc(g->n * sizeof(int));
	
	free(rows);
	free(cols);

    return g;
}


void dealloc_graph(graph* g){
    free(g->csc->ind);
    free(g->csc->ptr);
    free(g->csc);
    free(g->csr->ind);
    free(g->csr->ptr);
    free(g->csr);
    free(g->colors);
    free(g->removed);
    free(g->scc);
    free(g);
}


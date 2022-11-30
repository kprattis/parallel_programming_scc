#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "sparse.h"

typedef struct graph {
    int nnz;
    //number of non zero elements in adjacency matrix
    
    int n;
    //number of nodes

    sparse* csc;
    //graph represantation saved in CSC format:
    
    sparse* csr;
    //graph represantation saved in CSR format:
    
    int* colors;
    //colors of nodes
    
    int* removed;
    //flags for the nodes removed
	
    int is_empty;
    //1 if the graph has no nodes left
    
    int* scc;
    //for every node, it contains the index of the strongly connected component it belongs
    
    int n_scc;
    //number of SCC components
}graph;

graph* init_graph(FILE*);

void dealloc_graph(graph*);

#endif

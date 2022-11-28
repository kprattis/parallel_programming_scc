#include "omp_scc.h"
#include <omp.h>

void omp_pred(graph *g, int* frontier, int* nscc){
  
    int frontier_is_empty = 0;
	int** temp;
	int* next_frontier = (int*) calloc (g->n, sizeof(int));
	
	#pragma omp parallel for
	for(int i = 0; i < g->n; i++){
		if(frontier[i]){
			g->removed[i] = 1;
			g->scc[i] = nscc[g->colors[i]];
		}
	}

    while(!frontier_is_empty){
        frontier_is_empty = 1;
		#pragma omp parallel for
		for(int i = 0; i < g->n; i++){
            if(frontier[i]){
				int start = g->csr->ptr[i];
				int end = g->csr->ptr[i + 1];
		   		frontier[i] = 0;
				
				#pragma omp parallel for
				for(int j = start; j < end; j++){
				int parent = g->csr->ind[j];
						
					if(g->removed[parent]){
						continue;
					}
						
					if(g->colors[parent] == g->colors[i]){
						g->removed[parent] = 1;
						g->scc[parent] = nscc[g->colors[i]];
						next_frontier[parent] = 1;
						frontier_is_empty = 0;
					}
				
				}
            }
        }

		temp = &frontier;
		frontier = next_frontier;
		next_frontier = *temp;
	}
	free(next_frontier);
    
    
}

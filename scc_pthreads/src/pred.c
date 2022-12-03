#include "pthreads_scc.h"

void pred(int* frontier, int* nscc){
  
    int frontier_is_empty = 0;
	int** temp;
	int* next_frontier = (int*) calloc (g->n, sizeof(int));

	//initialize frontier
	for(int i = 0; i < NTHREADS; i++){
		args[i].id = i;
		args[i].unique = &frontier;
		args[i].n_scc = &nscc;

		pthread_create(&threads[i], &attr, init_frontier, (void *)&args[i]);
	}

	for(int i = 0; i < NTHREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	//perform multiple root bfs
    while(!frontier_is_empty){
        frontier_is_empty = 1;

		for(int i = 0; i < NTHREADS; i++){
			args[i].id = i;
			args[i].unique = &frontier;
			args[i].next_frontier = &next_frontier;
			args[i].n_scc = &nscc;
			args[i].flag = &frontier_is_empty;

			pthread_create(&threads[i], &attr, visit_parents, (void *)&args[i]);
		}

		for(int i = 0; i < NTHREADS; i++){
			pthread_join(threads[i], NULL);
		}

		//swap pointers
		temp = &frontier;
		frontier = next_frontier;
		next_frontier = *temp;

	}
	free(next_frontier);
    
}

void *init_frontier(void *p){
	int **frontier = ((param *)p)->unique;
	int **nscc = ((param *)p)->n_scc;
	int tid = ((param *)p)->id;

	for(int i = tid; i < g->n; i += NTHREADS){
		if((*frontier)[i]){
			g->removed[i] = 1;
			g->scc[i] = (*nscc)[g->colors[i]];
		}
	}
}

void *visit_parents(void *p){
	int **frontier = ((param *)p)->unique;
	int **nscc = ((param *)p)->n_scc;
	int *frontier_is_empty = ((param *)p)->flag; 
	int **next_frontier = ((param *)p)->next_frontier;
	int tid = ((param *)p)->id;

	for(int i = tid; i < g->n; i += NTHREADS){
            if((*frontier)[i]){
				(*frontier)[i] = 0;
				int start = g->csr->ptr[i];
				int end = g->csr->ptr[i + 1];
		   		
				for(int j = start; j < end; j++){
					
					int parent = g->csr->ind[j];
						
					if(g->removed[parent])
						continue;
						
					if(g->colors[parent] == g->colors[i]){
						g->removed[parent] = 1;
						g->scc[parent] = (*nscc)[g->colors[parent]];
						(*next_frontier)[parent] = 1;
						*frontier_is_empty = 0;
					}
				}
            }
        }
}

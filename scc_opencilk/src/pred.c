#include "cilk_scc.h"
#include "cilk/cilk.h"
#include "cilk/cilk_api.h"
#include <stdlib.h>

void pred(graph *g, int *frontier, int *nscc){
	/*
		Performs backwards BFS to find the sccs created 
		by the nodes who "won" in coloring the graph.
		It creates all the sccs simultaneously, instead of one at a time.

		Input:
			graph *g: the graph structure with the information of the graph
			int *frontier: at every iteration contains the 
						   nodes last visited/ the roots of the next iteration.
						   Initially contains all the 'winners' of the coloring
			int *nscc: maps every color with the corresponding scc ID
	
	*/

	int p = __cilkrts_get_nworkers();
    int pid;  

    //array of flags, so that every cilk worker writes its own result without races
    int* p_flags = (int*) calloc (p,  sizeof(int));

    int frontier_is_empty = 0;
	int **temp;
	int *next_frontier = (int*) calloc (g->n, sizeof(int));

	//Remove the initial nodes and place them in their corresponding scc
	cilk_for(int i = 0; i < g->n; i++){
		if(frontier[i]){
			g->removed[i] = 1;
			g->scc[i] = nscc[g->colors[i]];
		}
	}
	
    while(!frontier_is_empty){
        frontier_is_empty = 1;
        
		cilk_for(int i = 0; i < p; i++)
    		p_flags[i] = 0;

		//Loop over the nodes
        cilk_for(int i = 0; i < g->n; i++)
			//If the node is in the frontier
            if(frontier[i]){

				//Remove it from the frontier
				frontier[i] = 0;
				
				int start = g->csr->ptr[i];
				int end = g->csr->ptr[i + 1];
		   		//Loop over its parents
				cilk_for(int j = start; j < end; j++){
					int parent = g->csr->ind[j];
						
					if(g->removed[parent])
						continue;
					
					pid = __cilkrts_get_worker_number();

					//If the parent has the same color as the current node
					if(g->colors[parent] == g->colors[i]){
						//Remove it from the graph
						g->removed[parent] = 1;
						
						//Place it in the same scc
						g->scc[parent] = nscc[g->colors[i]];
						
						//Update the frontier of the next iteration
						next_frontier[parent] = 1;
						
						p_flags[pid] = 1;
						//frontier_is_empty = 0;
					}
				}
            }

		//gather flags of workers
		for(int i = 0; i < p; i++)
    		if(p_flags[i]){
            	frontier_is_empty = 0;
				//if at least one worker's flag has changed
				break;
        	}

        
		//swap the pointers of the next and the current frontier
		temp = &frontier;
		frontier = next_frontier;
		next_frontier = *temp;

	}

	//free memory
	free(next_frontier);
	free(p_flags);
    
}

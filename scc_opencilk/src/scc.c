#include <stdio.h>
#include <stdlib.h>
#include "cilk_scc.h"
#include "cilk/cilk.h"
#include "cilk/cilk_api.h"
#include <time.h>

int scc(FILE* f, int **SCC_arr){	
	/*
		Finds the sccs of a graph in .mtx format

		Input: 
			FILE *f = .mtx file containing the graph
			int** SCC_arr = address of array to store the calculated sccs

		Output:
			Returns the number of sccs
	*/
	int p = __cilkrts_get_nworkers();
    int pid;  

    //array of flags, so that every cilk worker writes its own result without races
    int* p_flags = (int*) calloc (p,  sizeof(int));

	//structure to keep time for various parts of the algorithm
	struct timespec begin, end; 
	double elapsed[2] = {0.0};

	//init the graph
	graph *g = init_graph(f);

	int* n_scc = (int*) calloc(g->n, sizeof(int));
	int* unique = (int*) calloc(g->n, sizeof(int));
	int n_unique;
	int changed_color;
	
	
	//trim the graph once to eliminate all trivial nodes
	int n_trimmed = trim(g);
	
	g->is_empty = (n_trimmed == g->n);

	while(!g->is_empty){
		
		//init colors for all the remaining nodes as their id
		cilk_for(int i = 0; i < g->n; i++){
			if(!g->removed[i])
				g->colors[i] = i;
		}

		clock_gettime(CLOCK_REALTIME, &begin);
	
			// perform BFS to push every node's color forward to 
			// its neighbors until no color changes
			changed_color = 1;	

			while(changed_color){
				changed_color = push_colors(g);
			}
		
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[0] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
		
		//  First, find all unique colors and preallocate the scc Id of every group.
		//  Next find the predecessors of the nodes whose color equals their id.
		//  Each such group forms an scc with the appropriate Id. Remove them from the graph.
		clock_gettime(CLOCK_REALTIME, &begin);
			
			//Find unique colors
			for(int i = 0; i < g->n; i++)
				if(!g->removed[i])
					unique[g->colors[i]] = 1;
		
			n_unique = 0;
			
			//Count how many unique colors are there and assign an scc number (n_scc[i]) 
			//for every such color.

			//This section is not parallelized because there will be races
			//Possible with pthreads mutex_lock. 
			for(int i = 0; i < g->n; i++)
				if(unique[i]){
					n_scc[g->colors[i]] = g->n_scc + n_unique;
					n_unique ++;			
				}

			
			//Perform a backward BFS to form all sccs simultaneously. The initial "roots"
			//are the "unique" nodes - whose color == their id.
			pred(g, unique, n_scc);

		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[1] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;


		//check if all nodes are removed and prepare for the next iteration
		g->is_empty = 1;
		cilk_for(int i = 0; i < g->n; i++){
			unique[i] = 0;
			n_scc[i] = 0;
			pid = __cilkrts_get_worker_number();
			if(!g->removed[i]){
				//raise flag that a node that is not removed was found
				p_flags[pid] = 1;
				
			}
		}

		//join every worker's result from the flag array to the g->is_empty variable
    	for(int i = 0; i < p; i++){
    		if(p_flags[i]){
            	g->is_empty = 0;
				//if at least one worker's flag has changed
				
        	}
			p_flags[i] = 0;
		}

		g->n_scc += n_unique;

    }
 	
	//Print time statistics
	printf("%lf, ",elapsed[0]);
	printf("%lf, ",elapsed[1]);

	//Create return values
	(* SCC_arr) = (int *) malloc(g->n * sizeof(int)); 
 	cilk_for(int i = 0; i < g->n; i++)
 		(*SCC_arr)[i] = g->scc[i];
 	
 	int NSCC = g->n_scc;

	//free allocated memory
    free(f);
	free(p_flags);
    free(n_scc);
    free(unique);
    dealloc_graph(g);

   	return NSCC; 
}

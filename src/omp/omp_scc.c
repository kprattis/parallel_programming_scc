#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include "omp_scc.h"

int omp_scc(FILE* f, int** SCC_arr){	
	
	graph *g = init_graph(f);
	
	int* n_scc = (int*) calloc(g->n, sizeof(int));
	int* unique = (int*) calloc(g->n, sizeof(int));
	int n_unique;
		
	struct timespec begin, end; 
	double elapsed[3] = {0.0}; 
	
	clock_gettime(CLOCK_REALTIME, &begin);
	omp_trim(g);
	clock_gettime(CLOCK_REALTIME, &end);
	elapsed[0] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;

	while(!g->is_empty){
		
		printf("Now scc = %d\n", g->n_scc); 
		#pragma omp parallel for
			for(int i = 0; i < g->n; i++){
				if(! g->removed[i])
					g->colors[i] = i;
			}
		
		clock_gettime(CLOCK_REALTIME, &begin);
		int changed_color = 1;	
		while(changed_color){
		    changed_color = omp_push_colors(g);
		}
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[1] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
		
		printf("Colored\n");
		
		/*  Find the predecessors of the nodes whose color equals their id.
		    Each such group forms a scc. Remove them from the graph.*/
		clock_gettime(CLOCK_REALTIME, &begin);
			#pragma omp parallel for
				for(int i = 0; i < g->n; i++)
					if(!g->removed[i])
						unique[g->colors[i]] = 1;	
			n_unique = 0;

			#pragma omp parallel for
				for(int i = 0; i < g->n; i++){
					if(unique[i]){
						n_scc[g->colors[i]] = g->n_scc + n_unique;
						
						#pragma omp atomic 
							n_unique ++;

					}
				}	
			

		omp_pred(g, unique, n_scc);
		
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[2] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;

		g->is_empty = 1;
		#pragma omp parallel for
			for(int i = 0; i < g->n; i++){
				unique[i] = 0;
				n_scc[i] = 0;
				if(!g->removed[i]){
					g->is_empty = 0;
					
				}
			}

		g->n_scc += n_unique;
    }
 	
	printf("trim:%lf\n",elapsed[0]);
	printf("color:%lf\n",elapsed[1]);
	printf("pred:%lf\n",elapsed[2]);

	(* SCC_arr) = (int *) malloc(g->n * sizeof(int)); 
	#pragma omp parallel for
 		for(int i = 0; i < g->n; i++)
 			(*SCC_arr)[i] = g->scc[i];
	int NSCC = g->n_scc;

	free(f);
	free(n_scc);
	free(unique);
	dealloc_graph(g);
   	return NSCC; 
}

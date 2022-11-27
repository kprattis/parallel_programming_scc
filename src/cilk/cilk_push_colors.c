#include <stdio.h>
#include "cilk_scc.h"
#include "cilk/cilk.h"
#include "cilk/cilk_api.h"

int push_colors(graph *g){
	int p = __cilkrts_get_nworkers();
    int changed_color = 0;
    int pid;
    
    int* p_changed_flags = (int*) calloc (p,  sizeof(int));
    
    cilk_for(int i = 0; i < g->n; i++){
        if(g->removed[i]){
            continue;
        }

        int start = g->csr->ptr[i];
        int end = g->csr->ptr[i + 1];
		
		pid = __cilkrts_get_worker_number();
        
        for(int j = start; j < end; j++){
            int parent = g->csr->ind[j];
            if(g->removed[parent]){
                continue;
	    }

            if(g->colors[i] > g->colors[parent]){
                g->colors[i] = g->colors[parent];
               	p_changed_flags[pid] = 1;
            }   
        }
    }
    
    cilk_for(int i = 0; i < p; i++){
    	if(p_changed_flags[i]){
    		changed_color = 1;
    		
    	}
    }
    
    free(p_changed_flags);
    return changed_color;
}

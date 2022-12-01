#include <stdlib.h>
#include "cilk_scc.h"
#include "cilk/cilk.h"
#include "cilk/cilk_api.h"


int push_colors(graph *g){
    /*
        Push colors from every node to their neighbors.
        Instead of pushing from parent to child, this code 
        simulates the "pulling" of the colors
        from the parents. To avoid races in the bool variable,
        occuring possibly due to compiler optimization, every cilk worker
        writes on its own part of a boolean array.  
    */

    int p = __cilkrts_get_nworkers();
    int pid;  

    //array of flags, so that every cilk worker writes its own result without races
    int* p_changed_flags = (int*) calloc (p,  sizeof(int));
    
    //return if color changed
    int changed_color = 0;
   
    //for every node
    cilk_for(int i = 0; i < g->n; i++){
        if(g->removed[i])
            continue;

        int start = g->csr->ptr[i];
        int end = g->csr->ptr[i + 1];

        //Id of current cilk worker
        pid = __cilkrts_get_worker_number();

        //loop over all the parents
        for(int j = start; j < end; j++){
            int parent = g->csr->ind[j];
            
            if(g->removed[parent])
                continue;

            //keep the min color
            if(g->colors[i] > g->colors[parent]){
                g->colors[i] = g->colors[parent];
                p_changed_flags[pid] = 1;
            }   
        }
    }

    //join every worker's result from the flag array to the changed_color
    for(int i = 0; i < p; i++)
    	if(p_changed_flags[i]){
            changed_color = 1;

            //if at least one worker found that color has changed
            break;
        }
    		

    //free flag array
    free(p_changed_flags);

    return changed_color;
}
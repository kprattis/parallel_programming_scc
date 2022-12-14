#include <stdio.h>
#include "seq_scc.h"

int push_colors(graph *g){
    /*
        Push colors from every node to their neighbors.
        Instead of pushing from parent to child, this code 
        simulates the "pulling" of the colors
        from the parents.   
    */

    //return if color changed
    int changed_color = 0;
   
    for(int i = 0; i < g->n; i++){
        if(g->removed[i])
            continue;

        int start = g->csr->ptr[i];
        int end = g->csr->ptr[i + 1];

        //loop over all the parents
        for(int j = start; j < end; j++){
            int parent = g->csr->ind[j];
            
            if(g->removed[parent])
                continue;

            //keep the min color
            if(g->colors[i] > g->colors[parent]){
                g->colors[i] = g->colors[parent];
                changed_color = 1;
            }   
        }
    }
    return changed_color;
}

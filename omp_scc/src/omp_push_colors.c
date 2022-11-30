#include "omp_scc.h"
#include <omp.h>

int push_colors(graph *g){
    
    int changed_color = 0;

    #pragma omp parallel for
    for(int i = 0; i < g->n; i++){
        if(g->removed[i])
            continue;

        int start = g->csr->ptr[i];
        int end = g->csr->ptr[i + 1];
        
        
            
        int color = g->colors[i];
        
            for(int j = start; j < end; j++){
                int parent = g->csr->ind[j];
                if(g->removed[parent])
                    continue;

                color = (color > g->colors[parent]) ? g->colors[parent] : color;
            }
            
            
            if(g->colors[i] != color){
                g->colors[i] = color;
                changed_color = 1;
            }
        
    }
    return changed_color;
}

#include <stdio.h>
#include "pthreads_scc.h"
#include <pthread.h>

void *push_colors(void *p){

    int *changed_color = ((param *) p)->flag;
    int tid = ((param *) p)->id;

    *changed_color = 0;

    for(int i = tid; i < g->n; i += NTHREADS){
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
                *changed_color = 1;
            }   
        }
    }
}

#include "seq_scc.h"

int trim(graph *g){
    /*
        Performs "trimming" to the graph. Removes all trivial sccs

        Input: graph *g

        Output: nodes_trimmed

    */

    int n_trimmed = 0;
    
    //trim unti no more trivial sccs exist
    for(int i = 0; i < g->n; i++){

        //Degree of the node
        int in_deg = g->csr->ptr[i + 1] - g->csr->ptr[i];
        int out_deg = g->csc->ptr[i + 1] - g->csc->ptr[i];
        
        //The first neighbor to be used to check the case of a selflooop
        int first_neigh_in = g->csr->ind[g->csr->ptr[i]];
        int first_neigh_out = g->csc->ind[g->csc->ptr[i]];
    	
	
        //remove the node if it is a trivial scc
        if((in_deg == 0 || out_deg == 0) || (in_deg == 1 && first_neigh_in == i) || (out_deg == 1 && first_neigh_out == i)){
            g->removed[i] = 1;
		    g->scc[i] = g->n_scc;
            g->n_scc ++;
            n_trimmed ++;
        }
    }

    return n_trimmed;
}

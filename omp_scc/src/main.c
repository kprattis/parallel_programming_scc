#include <stdio.h>
#include <time.h>
#include "omp_scc.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: bin/seq_scc [Filename].mtx/n, ");
        printf("where [Filename] is the name of the .mtx file containing the graph.\n");
        exit(1);
    }
    
    FILE* f;
    if ((f = fopen(argv[1], "r")) == NULL){
		printf("File %s not found\n", argv[1]); 
        exit(1);
    }
	
    // struct to save time
	struct timespec begin, end; 
	
    //the result will be saved here
	int n_scc;
	int *scc_arr;
	
    //calculate scc of the graph
	clock_gettime(CLOCK_REALTIME, &begin);
		n_scc = scc(f, &scc_arr);
	clock_gettime(CLOCK_REALTIME, &end);	
    double elapsed = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
	
	printf("OpenMP: There are %d SCC in the graph %s, found in %lf seconds.\n", n_scc, argv[1], elapsed);
    
    return 0;
}

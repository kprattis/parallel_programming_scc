#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp_scc.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: bin/seq_scc [Filename].mtx/n, ");
        printf("where [Filename] is the name of the .mtx file containing the graph.\n");
        exit(1);
    }
    
    FILE* f;
    if ((f = fopen(argv[1], "r")) == NULL){
		printf("File %s not found\n", argv[1]); 
        exit(1);
    }
	
    if(argc == 3){
        omp_set_num_threads(atoi(argv[2]));
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
	
	printf("%lf, %d, ",elapsed, n_scc);
    
    return 0;
}

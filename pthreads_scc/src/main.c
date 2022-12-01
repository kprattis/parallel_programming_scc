#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pthreads_scc.h"
#include <pthread.h>

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
	
    //define external variables
    pthread_attr_t attr;
    pthread_mutex_t mut;
    pthread_cond_t available_thread;

    pthread_mutex_init(&mut, NULL);
	pthread_attr_init(&attr);
	pthread_cond_init(&available_thread, NULL);

    pthread_t *threads = (pthread *) malloc(NTHREADS * sizeof(pthread_t));
    param *args = (param *) malloc(NTHREADS * sizeof(param));

    // struct to save time
	struct timespec begin, end; 
	
    //Here the result will be saved
	int n_scc;
	int *scc_arr;
	
    //calculate scc of the graph
	clock_gettime(CLOCK_REALTIME, &begin);
		n_scc = scc(f, &scc_arr);
	clock_gettime(CLOCK_REALTIME, &end);	
    double elapsed = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
	
	printf("Pthreads: There are %d SCC in the graph %s, found in %lf seconds.\n", n_scc, argv[1], elapsed);

    //free memory
    pthread_mutex_destroy(&mut);
    free(mut);
    pthread_cond_destroy(&available_thread);
    free(available_thread);
    pthread_attr_destroy(&attr);
    free(attr);
    
    free(args);
    free(threads);    
    return 0;
}

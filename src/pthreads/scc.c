#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pthreads_scc.h"
#include <pthread.h>

void *mark_unique(void *p);
void* init_colors(void* p);
void *n_scc_calc(void *p);
//void* do_work(void *);

int scc(FILE* f, int** SCC_arr){	
	
	g = init_graph(f);
	struct timespec begin, end; 
	double elapsed[3] = {0.0}; //= (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
	
	pthread_t* threads = (pthread_t *)malloc(sizeof(pthread_t)*NTHREADS);
	int front = 0;
	int rear = NTHREADS - 1;
	int activeThreads = 0;

	pthread_mutex_init(&mut, NULL);
	pthread_attr_init(&attr);
	pthread_cond_init(&available_thread, NULL);


	param *p = (param *) malloc(sizeof(param)*NTHREADS);

	int* n_scc = (int*) calloc(g->n, sizeof(int));
	int* unique = (int*) calloc(g->n, sizeof(int));
	int n_unique;
	
	clock_gettime(CLOCK_REALTIME, &begin);
	trim(g);
	clock_gettime(CLOCK_REALTIME, &end);
	elapsed[0] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;

	while(!g->is_empty){
		
		printf("Now scc = %d\n", g->n_scc); 
		for(int i = 0; i < NTHREADS; i++){
			p[i].id = i;
			pthread_create(&threads[i], &attr, &init_colors, (void *)&p[i]);
		}

		for(int i = 0; i < NTHREADS; i++){
			pthread_join(threads[i],NULL);
		}

		clock_gettime(CLOCK_REALTIME, &begin);
	
		int changed_color = 1;	

		while(changed_color){
			changed_color = 0;
		    for(int i = 0; i < NTHREADS; i++){
				p[i].id = i;
				p[i].flag = &changed_color;
				pthread_create(&threads[i], &attr, &push_colors, (void *)&p[i]);
			}

			for(int i = 0; i < NTHREADS; i++){
				pthread_join(threads[i], NULL);
			}
		}
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[1] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;
		printf("Colored\n");
		
		/*  Find the predecessors of the nodes whose color equals their id.
		    Each such group forms a scc. Remove them from the graph.*/
		clock_gettime(CLOCK_REALTIME, &begin);
		
		for(int i = 0; i < NTHREADS; i++){
			p[i].id = i;
			p[i].unique = &unique;
			pthread_create(&threads[i], &attr, &mark_unique, (void *)&p[i]);
		}


		for(int i = 0; i < NTHREADS; i++){
			pthread_join(threads[i], NULL);
		}
		
		n_unique = 0;
		
		for(int i = 0; i < NTHREADS; i++){
			p[i].id = i;
			p[i].unique = &unique;
			p[i].n_scc = &n_scc;
			p[i].n_unique = &n_unique;
			pthread_create(&threads[i], &attr, &n_scc_calc, (void *)&p[i]);
		}

		for(int i = 0; i < NTHREADS; i++){
			pthread_join(threads[i], NULL);
		}

		
		pred(g, unique, n_scc);
		
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed[2] += (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)*1e-9;

		g->is_empty = 1;
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
 	for(int i = 0; i < g->n; i++)
 		(*SCC_arr)[i] = g->scc[i];
 	
 	int NSCC = g->n_scc;
    free(f);
    free(n_scc);
	free(p);
    free(unique);
    dealloc_graph(g);
   return NSCC; 
}


void *init_colors(void *p){
	
	int tid = ((param *) p )->id;
	for(int i = tid; i < g->n; i+= NTHREADS){
		if(! g->removed[i])
			g->colors[i] = i;
	}
} 

void *mark_unique(void *p){
	int **unique = ((param *)p)->unique;
	int tid = ((param *)p)->id;
	for(int i = tid; i<g->n; i+=NTHREADS){
		if(!g->removed[i])
		(*unique)[g->colors[i]] = 1;
	}
}

void *n_scc_calc(void *p){
	int **unique = ((param *)p)->unique;
	int **n_scc = ((param *)p)->n_scc;
	int *n_unique = ((param *)p)->n_unique;
	int tid = ((param *)p)->id;

	for(int i = tid; i<g->n; i+=NTHREADS){
		if((*unique)[i]){
			(* n_scc)[g->colors[i]] = g->n_scc + *n_unique;

		pthread_mutex_lock(&mut);	
			(*n_unique) ++;		
		pthread_mutex_unlock(&mut);	
		
		}
	}	
}

/*
void *do_work(void *args){
	param *p = (param*) args;
	task *t ;
	param *A;
	while(! *(p->work_done)){
		printf("---------------------------------------------------------\n");
		t = pop_task(p->q);
		A = (param*) (t->args);
		A->id = p->id;
		//printf("I am thread %d and now I have node %d\n", p->id, A->node);
		(* (t->function))((void*) A);
		//printf("I am thread %d and now I return node %d\n", p->id,  A->node);
	}
	return p->g;
}
*/
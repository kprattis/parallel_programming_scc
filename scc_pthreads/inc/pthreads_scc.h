#ifndef PTHREAD_SCC_H
#define PTHREAD_SCC_H

#include "graph.h"
#include <pthread.h>


extern int NTHREADS;

//to be used in order to 
//pass the parameters to 
//pthread_create for various functions
typedef struct{
	int id;

    int* flag;
    int** unique;
    int *n_unique;
    int **n_scc;
    int **next_frontier;
}param;


//for easier to read code, g is a universal variable
extern graph* g;

//pthread variables
extern pthread_attr_t attr;
extern pthread_mutex_t mut;

extern pthread_t *threads;

extern param *args;

//functions
int scc(FILE *f, int **);

void pred(int *frontier, int *n_scc);

void *push_colors(void *);

int trim(void);

//extra functions to be used for pthreads
void *mark_unique(void *p);
void* init_colors(void* p);
void *n_scc_calc(void *p);
void *prepare_iter(void *p);

void *init_frontier(void *p);
void *visit_parents(void *p);

#endif

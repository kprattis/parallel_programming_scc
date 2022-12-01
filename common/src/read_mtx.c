#include <stdio.h>
#include <stdlib.h>
#include <read_mtx.h>
#include <mmio.h>

void read_mtx(FILE* f, int* N, int* nnz, int** rows, int** cols){
	/*
		Read the file f and save the matrix stored in coo format 
	*/

	MM_typecode matcode;
	int ret_code;
    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }
	/* find out size of sparse matrix .... */
	int M;
    if ((ret_code = mm_read_mtx_crd_size(f, &M, N, nnz)) !=0){
        exit(1);
	}
    *rows = (int *) malloc((*nnz) * sizeof(int));
    *cols = (int *) malloc((*nnz) * sizeof(int));

	double temp;
	if(!mm_is_pattern(matcode)){
		for (int i = 0; i < *nnz; i++){
		    fscanf(f, "%d %d %lg\n", &((*rows)[i]), &((*cols)[i]), &temp);
		    (*rows)[i]--;  /* adjust from 1-based to 0-based */
		    (*cols)[i]--;
		}
	
	}
	
	else{
			
		for (int i = 0; i < *nnz; i++){
		    fscanf(f, "%d %d\n", &((*rows)[i]), &((*cols)[i]));
		    (*rows)[i]--;  /* adjust from 1-based to 0-based */
		    (*cols)[i]--;
		}
	}
}

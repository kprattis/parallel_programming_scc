#include <stdio.h>
#include <stdlib.h>
#include <read_mtx.h>
#include <mmio.h>

void read_mtx(FILE* f, int* N, int* nnz, int** ind, int** ptr){
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
    *ind = (int *) malloc((*nnz) * sizeof(int));
    *ptr = (int *) malloc((*N + 1) * sizeof(int));

	int row, col, count;
	double temp;

	count = 0;
	(*ptr)[0] = 0;

	if(!mm_is_pattern(matcode)){

		for (int i = 0; i < *nnz; i++){
		    fscanf(f, "%d %d %lg\n", &row, &col, &temp);
		    (*ind)[i] = row - 1;  /* adjust from 1-based to 0-based */
			if( (col - 1) != (*ptr)[count]){
		    	count = count + 1;
				(*ptr)[count] = i;
			}
		}
	
	}
	
	else{
			
		for (int i = 0; i < *nnz; i++){
		    fscanf(f, "%d %d %lg\n", &row, &col);
		    (*ind)[i] = row - 1;  /* adjust from 1-based to 0-based */
			if( (col - 1) != (*ptr)[count]){
		    	count = count + 1;
				(*ptr)[count] = i;
			}
		}
	}
}

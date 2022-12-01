#include "sparse.h"

void coo2csc(int *rows, int *cols, sparse *adj, int n, int nnz){
	//assumes sorted cols	
	for(int i = 0; i < nnz; i++){
		adj->ind[i] = rows[i];
		adj->ptr[cols[i]] ++;
	}
	//accumulative sum
	int cum_sum = 0;
	int temp;
	for(int i = 0; i < n; i++){
		temp = adj->ptr[i];
		adj->ptr[i] = cum_sum;
		cum_sum += temp;	
	}
	adj->ptr[n] = nnz;
}

void csc2csr(sparse *csc, sparse *csr, int n, int nnz){	
	
	//count rows
	for(int i = 0; i < nnz; i++){			
		csr->ptr[csc->ind[i]] ++;
	}

	//accumulative sum
	int cum_sum = 0;
	int temp;
	for(int i = 0; i < n; i++){
		temp = csr->ptr[i];
		csr->ptr[i] = cum_sum;
		cum_sum += temp;	
		
	}
	csr->ptr[n] = nnz;
	
	//fill ind
	int row, dest;
	for(int i = 0; i < n; i++){
		for(int j = csc->ptr[i]; j < csc->ptr[i + 1]; j++){				
			row = csc->ind[j];
			dest = csr->ptr[row];
			csr->ind[dest] = i;
			csr->ptr[row] ++;
		}
	}

	//shift ptr to correct form
	int last = 0;
	for(int i = 0; i < (n + 1); i++){
		temp = csr->ptr[i];
		csr->ptr[i] = last;
		last = temp;
	}

}




#ifndef SPARSE_H
#define SPARSE_H

/*
    Data Structure to save a an adjacency matrix as a csr or csc format.
    ---------------------------------------------------------------------
    ptr -> Vector of size n+1, where n is the number 
            of nodes of the graph. 
    ind -> Vector of size nnz, where nnz is the number 
            of non zero elements in the adjacency matrix.

    ind and ptr have values such that, for every i: 
        ptr[0] = 0
        
        If ptr[i] = j_1, ptr[i+1]=j_2
            => j_1 <= j_2 and {ind[j_1], ind[j_1 + 1], ..., ind[j_2 - 1]} are the 
                                parents/childs of node i (depending on whether the 
                                struct represents a csr/csc format)

*/

typedef struct sparse{
    int* ptr;
    int* ind;
}sparse;

//transform coordinate format to csc
void coo2csc(int *, int *, sparse *, int, int);

//transform coordinate format to csr
void csc2csr(sparse *, sparse *, int, int);

#endif

#ifndef SPARSE_H
#define SPARSE_H

typedef struct sparse{
    int* ptr;
    int* ind;
}sparse;

void coo2csc(int *, int *, sparse *, int, int);

void csc2csr(sparse *, sparse *, int, int);

#endif

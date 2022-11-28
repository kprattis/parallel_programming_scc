CC = ../parallel/SCC/opt/opencilk/bin/clang
CC2 = gcc

SEQ = seq
CILK = cilk
OMP = omp

INC = inc
SRC = src
OBJ_DIR = obj
BIN = bin
PTHREADS = pthreads

OMP_OUT = $(BIN)/omp_scc
CILK_OUT = $(BIN)/cilk_scc
SEQ_OUT = $(BIN)/seq_scc
PTHREADS_OUT = $(BIN)/pthreads_scc

CFLAGS = -Wall -g -O3 -I $(INC)

CFILES = $(wildcard $(SRC)/*.c)
OBJ = $(CFILES:$(SRC)/%.c=$(OBJ_DIR)/%.o)

SEQ_CFILES = $(wildcard $(SRC)/$(SEQ)/*.c)
SEQ_OBJ = $(SEQ_CFILES:$(SRC)/$(SEQ)/%.c=$(OBJ_DIR)/$(SEQ)/%.o)

CILK_FLAGS = -fopencilk
CILK_CFILES = $(wildcard $(SRC)/$(CILK)/*.c)
CILK_OBJ = $(CILK_CFILES:$(SRC)/$(CILK)/%.c=$(OBJ_DIR)/$(CILK)/%.o)

OMP_CFILES = $(wildcard $(SRC)/$(OMP)/*.c)
OMP_OBJ = $(OMP_CFILES:$(SRC)/$(OMP)/%.c=$(OBJ_DIR)/$(OMP)/%.o)
OMP_FLAGS = -fopenmp

PTHREADS_CFILES = $(wildcard $(SRC)/$(PTHREADS)/*.c)
PTHREADS_OBJ = $(PTHREADS_CFILES:$(SRC)/$(PTHREADS)/%.c=$(OBJ_DIR)/$(PTHREADS)/%.o)
PTHREADS_FLAGS = -pthread

all: $(PTHREADS_OUT)

$(SEQ_OUT): $(OBJ) $(SEQ_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

$(CILK_OUT): $(OBJ) $(CILK_OBJ)
	$(CC) $(CILK_FLAGS) $(CFLAGS) -o $@ $^
	
$(OMP_OUT): $(OBJ) $(OMP_OBJ)
	$(CC2) $(OMP_FLAGS) $(CFLAGS) -o $@ $^

$(PTHREADS_OUT): $(OBJ) $(PTHREADS_OBJ)
	$(CC2) $(PTHREADS_FLAGS) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC)/%.c
	$(CC2) $(CFLAGS) $(PTHREADS_FLAGS) -o $@ -c $<
	
$(OBJ_DIR)/$(SEQ)/%.o : $(SRC)/$(SEQ)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

$(OBJ_DIR)/$(CILK)/%.o : $(SRC)/$(CILK)/%.c
	$(CC) $(CILK_FLAGS) $(CFLAGS) -o $@ -c $< 

$(OBJ_DIR)/$(OMP)/%.o : $(SRC)/$(OMP)/%.c
	$(CC2) $(OMP_FLAGS) $(CFLAGS) -o $@ -c $< 

$(OBJ_DIR)/$(PTHREADS)/%.o : $(SRC)/$(PTHREADS)/%.c
	$(CC2) $(PTHREADS_FLAGS) $(CFLAGS) -o $@ -c $< 

.PHONY: clean

clean:
	rm -rf $(OBJ)
	rm -rf $(SEQ_OBJ)
	rm -rf $(CILK_OBJ)
	rm -rf $(OMP_OBJ)
	rm -rf $(PTHREADS_OBJ)
	rm -rf $(BIN)/*



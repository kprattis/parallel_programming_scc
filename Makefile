CC = opt/opencilk/bin/clang

SEQ = seq
CILK = cilk

INC = inc
SRC = src
OBJ_DIR = obj
BIN = bin

CILK_OUT = $(BIN)/cilk_scc
SEQ_OUT = $(BIN)/seq_scc

CFLAGS = -Wall -g -O3 -I $(INC)

CFILES = $(wildcard $(SRC)/*.c)
OBJ = $(CFILES:$(SRC)/%.c=$(OBJ_DIR)/%.o)

SEQ_CFILES = $(wildcard $(SRC)/$(SEQ)/*.c)
SEQ_OBJ = $(SEQ_CFILES:$(SRC)/$(SEQ)/%.c=$(OBJ_DIR)/$(SEQ)/%.o)

CILK_FLAGS = -fopencilk
CILK_CFILES = $(wildcard $(SRC)/$(CILK)/*.c)
CILK_OBJ = $(CILK_CFILES:$(SRC)/$(CILK)/%.c=$(OBJ_DIR)/$(CILK)/%.o)

all: $(SEQ_OUT) $(CILK_OUT)

$(SEQ_OUT): $(OBJ) $(SEQ_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

$(CILK_OUT): $(OBJ) $(CILK_OBJ)
	$(CC) $(CILK_FLAGS) $(CFLAGS) -o $@ $^ 

$(OBJ_DIR)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) $(CILK_FLAGS) -o $@ -c $<
	
$(OBJ_DIR)/$(SEQ)/%.o : $(SRC)/$(SEQ)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

$(OBJ_DIR)/$(CILK)/%.o : $(SRC)/$(CILK)/%.c
	$(CC) $(CILK_FLAGS) $(CFLAGS) -o $@ -c $< 
	
.PHONY: clean

clean:
	rm -rf $(OBJ)
	rm -rf $(SEQ_OBJ)
	rm -rf $(BIN)/*



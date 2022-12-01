CC = gcc

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

COMMON = ../common

OUT = $(BIN_DIR)/pthreads_scc

INC = -I $(INC_DIR) -I $(COMMON)/$(INC_DIR)

CFLAGS = -Wall -g -O3 -lpthread $(INC)

CFILES = $(wildcard $(SRC_DIR)/*.c)
CFILES_CMN = $(wildcard $(COMMON)/$(SRC_DIR)/*.c) 
OBJ = $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) 
OBJ_CMN = $(CFILES_CMN:$(COMMON)/$(SRC_DIR)/%.c=$(OBJ_DIR)/common/%.o)

all: $(OUT)

$(OUT): $(OBJ) $(OBJ_CMN)
	$(CC) $(CFLAGS) -o $@ $^ 
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 
$(OBJ_DIR)/common/%.o: $(COMMON)/$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 
	
.PHONY: clean

clean:
	rm -rf $(OBJ)
	rm -rf $(OBJ_CMN)
	rm -rf $(OUT)


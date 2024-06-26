
CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
OBJ_DIR := $(CUR_DIR)/obj
CC := gcc
FLAG := -lpthread

mk_obj:
	$(CC) -c $(CUR_DIR)/main.c -o $(OBJ_DIR)/main.o 

all:    mk_obj
	$(CC) $(OBJ_DIR)/main.o -o $(BIN_DIR)/runfile $(FLAG)

run1:
	./bin/runfile 6000
run2:
	./bin/runfile 5000
run3:
	./bin/runfile 4000
clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*



CC=gcc
CFLAGS=-O3 -I. -g -Wall
LIBS=-lm

OBJ_=main.o
OBJ_DIR=obj
OBJ=$(patsubst %,$(OBJ_DIR)/%,$(OBJ_))

OUT_FILE=test

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

all: $(OBJ)
	$(CC) $^ -o $(OUT_FILE) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OUT_FILE) $(OBJ_DIR)/*.o *~
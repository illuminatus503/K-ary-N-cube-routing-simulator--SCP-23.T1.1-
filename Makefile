CC = gcc
CFLAGS = -g -Wall -O3

OBJ_DIR = obj
INCLUDE = -Iinclude
LIBS=-lm

_OBJ= main.o graph.o topologies.o 
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

SRC_DIR = src
IN_FILE = main
OUT_FILE = extra1

$(OUT_FILE): $(OBJ) # Link all object files.
	$(CC) -o $@ $^ $(INCLUDE) $(LIBS) 

$(OBJ_DIR)/$(IN_FILE).o: ./$(IN_FILE).c # Compile main.c
	$(CC) $(CFLAGS) -c $<  -o $@ $(INCLUDE) $(LIBS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c # Compile all header files.
	$(CC) $(CFLAGS) -c $<  -o $@ $(INCLUDE) $(LIBS) 

.PHONY: clean

clean:
	rm -f $(OUT_FILE) $(OBJ_DIR)/*.o *~
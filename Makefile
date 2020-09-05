CC = gcc
SRC_UTIL_DIR = src/util
SRC_UTIL = $(wildcard src/util/*.c)
SRC_CORE_DIR = src/core
SRC_CORE = $(wildcard src/core/*.c)
SRC_SOLVER_DIR = src/solver
SRC_SOLVER = $(wildcard src/solver/*.c)
OBJ_UTIL = $(SRC_UTIL:.c=.o)
OBJ_CORE = $(SRC_CORE:.c=.o)
OBJ_SOLVER = $(SRC_SOLVER:.c=.o)
SOLVER = maze_solver

$(SRC_UTIL_DIR)/%.o: $(SRC_UTIL_DIR)/%.c
	$(CC) -O3 -c $< -o $@

$(SRC_CORE_DIR)/%.o: $(SRC_CORE_DIR)/%.c
	$(CC) -O3 -c $< -o $@

$(SRC_SOLVER_DIR)/%.o: $(SRC_SOLVER_DIR)/%.c
	$(CC) -O3 -c $< -o $@

$(SOLVER): $(OBJ_SOLVER) $(OBJ_CORE) $(OBJ_UTIL)
	$(CC) $(OBJ_CORE) $(OBJ_UTIL) $(OBJ_SOLVER) -o $(SOLVER)


.PHONY: clean
clean: $(SOLVER)
	@rm $(OBJ_UTIL)
	@rm $(OBJ_CORE)
	@rm $(OBJ_SOLVER)
	@rm $(SOLVER)

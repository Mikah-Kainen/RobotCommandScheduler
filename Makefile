# https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
# https://stackoverflow.com/questions/22545029/makefile-how-to-correctly-include-header-file-and-its-directory

.RECIPEPREFIX = >
OBJ_DIR = obj
LIB_DIR = lib
INC_DIR = inc
SRC_DIR = src
ccs = $(shell find $(SRC_DIR) -name '*.cc')
objects = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(ccs))

#ccs = $(wildcard $(SRC_DIR)/**/*.cc) $(MAIN_FILE)
#objects = $(patsubst **/%.cc, **/%.o, $(ccs))

CXX = g++
CPPFLAGS = -c -Wall -I. -MD -MP

all: $(OBJ_DIR) $(objects)
> $(CXX) -o rcs $(objects)
#$(LIB_DIR)/libev3dev.a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
> @mkdir -p $(dir $@)
> $(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
> mkdir -p $(OBJ_DIR)

debug:
> @echo "ccs: $(ccs)"
> @echo "objects: $(objects)"

.PHONY: clean
clean:
> rm -f $(EXEC)
> $(shell find $(OBJ_DIR) -name '*.o' -exec rm -f {} +)
> $(shell find $(OBJ_DIR) -name '*.d' -exec rm -f {} +)

.PHONY: cleanD
cleanD:
> $(shell find $(OBJ_DIR) -name '*.d' -exec rm -f {} +)

-include $(shell find $(OBJ_DIR) -name '*.d')
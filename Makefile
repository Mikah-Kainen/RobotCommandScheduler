# https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
#https://stackoverflow.com/questions/22545029/makefile-how-to-correctly-include-header-file-and-its-directory


OBJ_DIR = obj
LIB_DIR = lib
INC_DIR = inc
SRC_DIR = src
cpps = $(wildcard $(SRC_DIR)/**/.cpp)
# could add something like $(wildcard $(SRC_DIR)/SystemMonitorSrc/*.cpp) if there are nested folders
objects = $(patsubst $(SRC_DIR)/**/%.cpp, $(OBJ_DIR)/**/%.o, $(cpps))
CXX = g++
CPPFLAGS = -c -Wall -I. -MD -MP


all : $(OBJ_DIR) $(objects)
    $(CXX) -o SystemMonitor $(objects)
#$(LIB_DIR)/libev3dev.a


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
  $(CXX) $(CPPFLAGS) -c $< -o $@


$(OBJ_DIR):
  mkdir -p $(OBJ_DIR)


.PHONY : clean
clean :
    rm $(objects)
    rm $(wildcard $(OBJ_DIR)/*.d)
#   rm $(wildcard $(OBJ_DIR)/<foldername>/*.d)


.PHONY : cleanD
cleanD :
    rm $(wildcard $(OBJ_DIR)/*.d)
#   rm $(wildcard $(OBJ_DIR)/<foldername>/*.d)


-include $(wildcard $(OBJ_DIR)/*.d)

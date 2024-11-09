#
#	Makefile do AM
#

COMPILADOR=g++
FLAGS=-g -Wall -pg -fopenmp
SRC_DIR=src
OBJ_DIR=obj
EXEC_NAME = timetable
OBJS = $(addprefix $(OBJ_DIR)/, Constraints.o Courses.o Curricula.o heuristicas.o Rooms.o util.o)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(COMPILADOR) $(FLAGSC) -c $< -o $@

all: $(OBJS)
	$(COMPILADOR) $(FLAGS) $(OBJS) $(SRC_DIR)/main.cpp -o $(EXEC_NAME)

clean:
	rm -rf $(OBJ_DIR) $(EXEC_NAME) $(EXEC_NAME).tar.gz

$(OBJ_DIR):
	if test ! -d $(OBJ_DIR); then mkdir -p $(OBJ_DIR); fi


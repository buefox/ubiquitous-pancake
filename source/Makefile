CC = g++
CFLAGS = -c -Wall -std=c++11
INC_DIR = ../include/
OBJ_DIR = ../object/
SOURCES = User.cpp Application.cpp Edge.cpp Server.cpp Graph.cpp main.cpp
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SOURCES)) #$(SOURCES:.cpp=.o)
EXEC = main
IN_GEN = inputGenerator

ITER = 100

IN_DIR = ../input/
INPUTS = Applications.txt Users.txt Edges.txt Servers.txt

all: $(EXEC) $(IN_GEN)

$(EXEC): $(OBJECTS) $(IN_GEN)
	$(CC) -I $(INC_DIR) $(OBJECTS) -o $@

$(OBJ_DIR)%.o: %.cpp
	$(CC) -I $(INC_DIR) $(CFLAGS) $< -o $@

$(IN_GEN): $(IN_GEN).cpp 
	$(CC) $(CFLAGS) $< -o $@

run: $(EXEC)
	./$(EXEC) $(ITER) $(addprefix $(IN_DIR), $(INPUTS))

clean:
	rm -rf $(OBJECTS) $(EXEC) $(IN_GEN) *~ .*~
	clear

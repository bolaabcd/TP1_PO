INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

CC = g++
CFLAGS = -Wall -c -I$(INCLUDE_FOLDER) -O2
LIBS = -lgmp

# Arquivos
TARGET = $(BIN_FOLDER)/tp1.out
HDR_FILES = $(INCLUDE_FOLDER)/solution.hpp $(INCLUDE_FOLDER)/tableau.hpp
OBJ_FILES = $(OBJ_FOLDER)/solution.o $(OBJ_FOLDER)/tableau.o

build: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(LIBS) $(OBJ_FILES)

$(OBJ_FOLDER)/main.o: $(HDR_FILES) $(SRC_FOLDER)/main.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/main.o $(SRC_FOLDER)/main.cpp 

$(OBJ_FOLDER)/solution.o: $(HDR_FILES) $(SRC_FOLDER)/solution.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/solution.o $(SRC_FOLDER)/solution.cpp

$(OBJ_FOLDER)/tableau.o: $(HDR_FILES) $(SRC_FOLDER)/tableau.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/tableau.o $(SRC_FOLDER)/tableau.cpp 

clean:
	rm $(TARGET) $(OBJ_FILES)


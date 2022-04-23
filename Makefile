INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

CC = g++
CFLAGS = -Wall -c -I$(INCLUDE_FOLDER) -O2 #-O3
DBGFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++17 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts -c -I$(INCLUDE_FOLDER)
DBGLFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++17 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts -I$(INCLUDE_FOLDER)
LIBS = -lgmp

# Arquivos
TARGET = $(BIN_FOLDER)/tp1.out
HDR_FILES = $(INCLUDE_FOLDER)/tableau.hpp $(INCLUDE_FOLDER)/solution.hpp
OBJ_FILES = $(OBJ_FOLDER)/main.o $(OBJ_FOLDER)/tableau.o $(OBJ_FOLDER)/solution.o

build: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(DBGLFLAGS) $(LIBS) $(OBJ_FILES)

$(OBJ_FOLDER)/main.o: $(HDR_FILES) $(SRC_FOLDER)/main.cpp
	$(CC) $(DBGFLAGS) -o $(OBJ_FOLDER)/main.o $(SRC_FOLDER)/main.cpp 

$(OBJ_FOLDER)/tableau.o: $(HDR_FILES) $(SRC_FOLDER)/tableau.cpp
	$(CC) $(DBGFLAGS) -o $(OBJ_FOLDER)/tableau.o $(SRC_FOLDER)/tableau.cpp 

$(OBJ_FOLDER)/solution.o: $(HDR_FILES) $(SRC_FOLDER)/solution.cpp
	$(CC) $(DBGFLAGS) -o $(OBJ_FOLDER)/solution.o $(SRC_FOLDER)/solution.cpp

clean:
	rm $(TARGET) $(OBJ_FILES)


INCLUDE_FOLDER = ../include/

CC = g++
CFLAGS = -Wall -c -I$(INCLUDE_FOLDER) -O2 #-O3
DBGFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++17 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts -c -I$(INCLUDE_FOLDER)
DBGLFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++17 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts -I$(INCLUDE_FOLDER)
LIBS = -lgmp


get_all: checker.cpp gen.cpp
	$(CC) checker.cpp -o checker.out $(DBGLFLAGS) $(LIBS)
	$(CC) gen.cpp -o gen.out $(DBGLFLAGS) $(LIBS)

clean:
	rm checker.out gen.out


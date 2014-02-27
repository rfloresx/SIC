CC = g++
G = -g

FILE = main.cpp StringLib.h terminal.h assembler.h
OUT_FILE = assemble
build: $(FILE)
	$(CC) -o $(OUT_FILE) $(FILE)
clear:
	rm -f *.o core

rebuild: clean build

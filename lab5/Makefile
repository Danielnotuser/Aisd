PREF_SRC = ./src/
PREF_EXE = ./exe/

all:
	gcc $(PREF_SRC)*.c -o $(PREF_EXE)lab -g
	valgrind --leak-check=full $(PREF_EXE)lab

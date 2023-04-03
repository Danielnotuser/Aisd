PREF_MEM = ./lab3a/
PREF_FILE = ./lab3b/

mem:
	gcc -o a_lab $(PREF_MEM)*.c -g
	valgrind ./a_lab
file:
	gcc -o b_lab $(PREF_FILE)*.c -g
	valgrind ./b_lab
clean:
	rm a_lab b_lab

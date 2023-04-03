PREF_MEM = ./lab3v/
PREF_FILE = ./lab3g/

mem:
	gcc -o v_lab $(PREF_MEM)*.c -g
	valgrind ./v_lab
file:
	gcc -o g_lab $(PREF_FILE)*.c -g
	valgrind ./g_lab
clean:
	rm v_lab g_lab

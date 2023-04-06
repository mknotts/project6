all: psort.o

psort.o: psort.c
	gcc psort.c -o psort.o -Wall 

run:
	./psort.o input.txt output.txt 4

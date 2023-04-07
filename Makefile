all: psort.o

psort.o: psort.c
	gcc psort.c -o psort.o -Wall 

run:
	./psort.o input2.txt output.txt 4

big: 
	./psort.o test.img output.txt 4

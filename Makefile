all: psort.o

psort.o: psort.c
	gcc psort.c -o psort.o -Wall 

run:
	./psort.o input2.txt output.txt 4

small:
	./psort.o test-small-in.txt output.txt 1

small2:
	./psort.o test-small2-in.txt output.txt 4

medium:
	./psort.o test-medium-in.txt output.txt 4

medium2:
	./psort.o test-medium2-in.txt output.txt 4

big: 
	./psort.o test.img output.txt 4

# Giorgos Geramoutsos, @csd3927
# assignment 1, hy345, 2023

all: main.o shell.o
	gcc main.o shell.o -o shell.out

main.o: main.c shell.h
	gcc -c main.c

shell.o: shell.c shell.h
	gcc -c shell.c

run:
	./shell.out

clean:
	rm -rf *.o *.out
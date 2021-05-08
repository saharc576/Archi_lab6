all: mypipe

mypipe: mypipe.o
	gcc -g -m32 -Wall -o mypipe mypipe.o

mypipe.o: mypipe.c
	gcc -g -m32 -Wall -c -o mypipe.o mypipe.c

.PHONY: clean

clean:
	rm -rf ./*.o mypipe
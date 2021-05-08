all: mypipeline

mypipeline: mypipeline.o
	gcc -g -m32 -Wall -o mypipeline mypipeline.o

mypipeline.o: mypipeline.c
	gcc -g -m32 -Wall -c -o mypipeline.o mypipeline.c

.PHONY: clean

clean:
	rm -rf ./*.o mypipeline
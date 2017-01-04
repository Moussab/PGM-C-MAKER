
all: tp


tp: tp.o
	gcc tp.o -o tp -lm

tp.o: tp.c
	gcc -c tp.c

clean:
	rm *o tp



run : tp
	./tp
tp : tp.c
	gcc -o tp tp.c
clean:
	rm -rf *.o 
	rm -rf img.pgm

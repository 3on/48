CC=gcc
CFLAGS=-I.
DEPS = src/levenshtein.h
OBJ = src/causes.o src/levenshtein.o 

all: causes.bin

causes.bin: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	find . -name "*.o" -delete

distclean:clean
	rm -f causes.bin
	rm -f network.js
	rm -f word.network

mac: all
	./causes.bin
	open http://careers.stackoverflow.com/3on answer.html
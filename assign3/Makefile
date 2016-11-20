CC=gcc
CFLAGS= -Wall -g

all: diskinfo disklist diskget diskput

diskinfo: diskinfo.c
	$(CC) diskinfo.c -o diskinfo

disklist: disklist.c
	$(CC) disklist.c -o disklist

diskget: diskget.c
	$(CC) diskget.c -o diskget

diskput: diskput.c
	$(CC) diskput.c -o diskput

.PHONY: clean
clean:
	rm -rf *.o *.exe

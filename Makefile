CC=g++
CFLAGS=-Wall -g -DDEBUG

all: diskinfo

diskinfo: diskinfo.o superblock.o fat.o
	$(CC) $(CFLAGS) -o diskinfo diskinfo.o superblock.o fat.o

diskinfo.o: diskinfo.cpp superblock.h
	$(CC) $(CFLAGS) -c diskinfo.cpp

superblock.o: superblock.cpp superblock.h
	$(CC) $(CFLAGS) -c superblock.cpp

fat.o: fat.cpp fat.h superblock.o superblock.h
	$(CC) $(CFLAGS) -c fat.cpp

clean:
	rm -rf *.o diskinfo

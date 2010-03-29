CC=g++
CFLAGS=-Wall -g -DDEBUG

all: diskinfo disklist

diskinfo: diskinfo.o superblock.o fat.o
	$(CC) $(CFLAGS) -o diskinfo diskinfo.o superblock.o fat.o

disklist: disklist.o superblock.o direntries.o
	$(CC) $(CFLAGS) -o disklist disklist.o superblock.o direntries.o

diskinfo.o: diskinfo.cpp superblock.h
	$(CC) $(CFLAGS) -c diskinfo.cpp

disklist.o: disklist.cpp superblock.h direntries.h
	$(CC) $(CFLAGS) -c disklist.cpp

superblock.o: superblock.cpp superblock.h
	$(CC) $(CFLAGS) -c superblock.cpp

fat.o: fat.cpp fat.h superblock.o superblock.h
	$(CC) $(CFLAGS) -c fat.cpp

direntries.o: direntries.cpp direntries.h
	$(CC) $(CFLAGS) -c direntries.cpp

clean:
	rm -rf *.o diskinfo disklist

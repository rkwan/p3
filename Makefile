CC=g++
CFLAGS=-Wall -g

all: diskinfo disklist diskget diskput

diskinfo: diskinfo.o superblock.o fat.o
	$(CC) $(CFLAGS) -o diskinfo diskinfo.o superblock.o fat.o

disklist: disklist.o superblock.o direntries.o rootdir.o
	$(CC) $(CFLAGS) -o disklist disklist.o superblock.o direntries.o rootdir.o

diskget: diskget.o superblock.o direntries.o rootdir.o fat.o
	$(CC) $(CFLAGS) -o diskget diskget.o superblock.o direntries.o rootdir.o fat.o

diskput: diskput.o superblock.o direntries.o rootdir.o fat.o
	$(CC) $(CFLAGS) -o diskput diskput.o superblock.o direntries.o rootdir.o fat.o

diskinfo.o: diskinfo.cpp superblock.h
	$(CC) $(CFLAGS) -c diskinfo.cpp

disklist.o: disklist.cpp superblock.h direntries.h
	$(CC) $(CFLAGS) -c disklist.cpp

diskget.o: diskget.cpp
	$(CC) $(CFLAGS) -c diskget.cpp

diskput.o: diskput.cpp
	$(CC) $(CFLAGS) -c diskput.cpp

superblock.o: superblock.cpp superblock.h
	$(CC) $(CFLAGS) -c superblock.cpp

fat.o: fat.cpp fat.h superblock.o superblock.h
	$(CC) $(CFLAGS) -c fat.cpp

direntries.o: direntries.cpp direntries.h
	$(CC) $(CFLAGS) -c direntries.cpp

rootdir.o: rootdir.cpp rootdir.h direntries.o direntries.h fat.o fat.h
	$(CC) $(CFLAGS) -c rootdir.cpp

clean:
	rm -rf *.o diskinfo disklist diskget mkfile.cc foo.txt disk.img.gz

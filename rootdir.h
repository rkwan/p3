#ifndef ROOTDIR_H
#define ROOTDIR_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <new>
#include <netinet/in.h>
#include <ctime>
#include "superblock.h"
#include "direntries.h"
#include "fat.h"

using namespace std;

class RootDir {
	private:
		unsigned int start;
		unsigned int blocks;
		int entries;
		DirEntries *root;

		int availableFAT(FILE *, Superblock);
		int availableEntry();
		void updateFDT(Superblock, int, char);
	public:
		RootDir(FILE *, Superblock);
		int fileExists(char *);
		void getFile(FILE *, Superblock, char *);
		void putFile(FILE *, FILE *, Superblock);
		void closeDir();
		void print();
};

#endif

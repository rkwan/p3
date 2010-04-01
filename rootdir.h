#ifndef ROOTDIR_H
#define ROOTDIR_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <new>
#include <netinet/in.h>
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

	public:
		RootDir(FILE *, Superblock);
		int fileExists(char *);
		void getFile(FILE *, Superblock, char *);
		void closeDir();
		void print();
};

#endif

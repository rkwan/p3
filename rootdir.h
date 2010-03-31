#ifndef ROOTDIR_H
#define ROOTDIR_H

#include <iostream>
#include <new>
#include <stdlib.h>
#include "superblock.h"
#include "direntries.h"

using namespace std;

class RootDir {
	private:
		unsigned int start;
		unsigned int blocks;
		unsigned int entries;
		DirEntries *root;
	public:
		RootDir(FILE *, Superblock);
		void closeDir();
		void print();
};

#endif

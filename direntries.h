#ifndef DIRENTRIES_H
#define DIRENTRIES_H

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <netinet/in.h>
#include "superblock.h"

using namespace std;

class DirEntries {
	private:
		char _status;
		unsigned int starting_block;
		unsigned int block_count;
		unsigned int file_size;
		unsigned char create_time[8];
		unsigned char modify_time[8];
		char file_name[31];
		unsigned char unused[6]; // may not be necessary

		FILE *fptr;
	public:
		DirEntries();
		void getEntry(FILE *, Superblock);
		void print();
		unsigned char status() {return _status;}
		unsigned int startingBlock(){return starting_block;}
		unsigned int blockCount(){return block_count;}
		unsigned int fileSize(){return file_size;}
		unsigned char *createTime(){return create_time;}
		unsigned char *modifyTime(){return modify_time;}
		char *filename(){return file_name;}

		void setStatus(char, int, Superblock *);
		void setStartingBlock(unsigned int, int, Superblock *);
		void setBlockCount(unsigned int, int, Superblock *);
		void setFilesize(unsigned int, int, Superblock *);
		void setCreateTime(unsigned char [8], int, Superblock *);
		void setModifyTime(int, Superblock *);
		void setFilename(char *, int, Superblock *);
};

#endif

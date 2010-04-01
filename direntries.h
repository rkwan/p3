#ifndef DIRENTRIES_H
#define DIRENTRIES_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <netinet/in.h>

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
	public:
		DirEntries();
		void getEntry(FILE *);
		void print();
		unsigned char status() {return _status;}
		unsigned int startingBlock(){return starting_block;}
		unsigned int blockCount(){return block_count;}
		unsigned int fileSize(){return file_size;}
		unsigned char *createTime(){return create_time;}
		unsigned char *modifyTime(){return modify_time;}
		char *filename(){return file_name;}
};

#endif

#ifndef FAT_H
#define FAT_H

#include <iostream>
#include <netinet/in.h>
#include "superblock.h"

using namespace std;

class FAT {
    private:
        unsigned int free_blocks;
        unsigned int reserved_blocks;
        unsigned int allocated_blocks;
        unsigned int last_blocks;
    public:
        FAT(Superblock, FILE *);
        void print();
        unsigned int freeBlocks() {return free_blocks;}
        unsigned int reservedBlocks() {return reserved_blocks;}
        unsigned int allocatedBlocks() {return allocated_blocks;}
        unsigned int lastBlocks() {return last_blocks;}
};

#endif

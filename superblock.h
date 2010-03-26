#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <iostream>
#include <netinet/in.h>

using namespace std;

class Superblock {
    private:
        char fs_id[9];
        short int block_size;
        unsigned int block_count;
        unsigned int fat_start;
        unsigned int fat_blocks;
        unsigned int root_start;
        unsigned int root_blocks;
    public:
        Superblock(FILE *);
        void print();
        char* fsid() {return fs_id;}
        short int blockSize() {return block_size;}
        unsigned int blockCount() {return block_count;}
        unsigned int fatStart() {return fat_start;}
        unsigned int fatBlocks() {return fat_blocks;}
        unsigned int rootStart() {return root_start;}
        unsigned int rootBlocks() {return root_blocks;}
};

#endif

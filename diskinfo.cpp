#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

Superblock::Superblock(FILE *fp) {
    unsigned int buffer, buffer_s;

    fseek(fp, 0, SEEK_SET);
    fread(&fs_id, 8, 1, fp);
    fs_id[8] = '\0';
    fread(&buffer_s, sizeof(short int), 1, fp);
    block_size = htons(buffer_s);
    fread(&buffer, sizeof(unsigned int), 1, fp);
    block_count = htonl(buffer);
    fread(&buffer, sizeof(unsigned int), 1, fp);
    fat_start = htonl(buffer);
    fread(&buffer, sizeof(unsigned int), 1, fp);
    fat_blocks = htonl(buffer);
    fread(&buffer, sizeof(unsigned int), 1, fp);
    root_start = htonl(buffer);
    fread(&buffer, sizeof(unsigned int), 1, fp);
    root_blocks = htonl(buffer);
}

void Superblock::print() {
    cout << "Super block information:" << endl;
    cout << "Filesystem ID: " << fs_id << endl; // Remember to take this out later.
    cout << "Block size: " << block_size << endl;
    cout << "Block count: " << block_count << endl;
    cout << "FAT starts: " << fat_start << endl;
    cout << "FAT blocks: " << fat_blocks << endl;
    cout << "Root directory start: " << root_start << endl;
    cout << "Root directory blocks: " << root_blocks << endl;
}

class FAT {
    private:
        unsigned int free_blocks;
        unsigned int reserved_blocks;
        unsigned int allocated_blocks;
        unsigned int used_blocks;
    public:
        FAT(Superblock, FILE *);
        void print();
        unsigned int freeBlocks() {return free_blocks;}
        unsigned int reservedBlocks() {return reserved_blocks;}
        unsigned int allocatedBlocks() {return allocated_blocks;}
        unsigned int usedBlocks() {return used_blocks;}
};

FAT::FAT(Superblock sb, FILE *fp) {
    unsigned int i, buffer;
    free_blocks = 0;
    reserved_blocks = 0;
    allocated_blocks = 0;
    used_blocks = 0;

    fseek(fp, sb.fatStart()*sb.blockSize(), SEEK_SET);
    for (i = 0; i < sb.fatBlocks()*(sb.blockSize()/4); i++) {
        fread(&buffer, 4, 1, fp);
        buffer = htonl(buffer);
        if (buffer == 0x00000000)
            free_blocks++;
        else if (buffer == 0x00000001)
            reserved_blocks++;
        else if (buffer == 0xffffffff)
            allocated_blocks++;
        else
            used_blocks++;
    }
}

void FAT::print() {
    cout << "FAT information:" << endl;
    cout << "Free Blocks: " << free_blocks << endl;
    cout << "Reserved Blocks: " << reserved_blocks << endl;
    cout << "Allocated Blocks: " << allocated_blocks << endl;
    cout << "Used Blocks: " << used_blocks << endl; // Remember to remove this before submitting.
}

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        fprintf(stderr, "Error: invalid argument(s)");
        exit(1);
    }
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: failed to open file: %s", argv[1]);
        exit(1);
    }
    Superblock sb (fp);
    FAT f (sb, fp);
    sb.print();
    cout << endl;
    f.print();
    fclose(fp);
    return 0;
}

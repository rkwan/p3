#include <stdio.h>
#include "superblock.h"

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
#if DEBUG
    cout << "Filesystem ID: " << fs_id << endl;
#endif
    cout << "Block size: " << block_size << endl;
    cout << "Block count: " << block_count << endl;
    cout << "FAT starts: " << fat_start << endl;
    cout << "FAT blocks: " << fat_blocks << endl;
    cout << "Root directory start: " << root_start << endl;
    cout << "Root directory blocks: " << root_blocks << endl;
}

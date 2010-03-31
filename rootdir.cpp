#include "rootdir.h"

RootDir::RootDir(FILE *fp, Superblock sb) {
	start = sb.rootStart();
	blocks = sb.rootBlocks();
	entries = sb.rootBlocks()*sb.blockSize()/64;

	root = new DirEntries[entries];
	/* root = (DirEntries **) malloc(sizeof(DirEntries *) * entries); */
#if DEBUG
	cout << "Start: " << start << endl;
	cout << "Blocks: " << blocks << endl;
	cout << "Entries: " << entries << endl;
#endif 
	unsigned int i;
	for (i = 0; i < entries; i++) {
		if (fseek(fp, start*512+(64*i), SEEK_SET) == -1) {
			cerr << "seek failed";
			exit(1);
		}
		root[i].getEntry(fp);
	}
}

void RootDir::closeDir() {
	delete [] root;
}

void RootDir::print() {
	unsigned int i;
	for (i = 0; i < entries; i++) {
		root[i].print();
	}
}

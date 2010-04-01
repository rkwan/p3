#include "rootdir.h"

RootDir::RootDir(FILE *fp, Superblock sb) {
	start = sb.rootStart();
	blocks = sb.rootBlocks();
	entries = sb.rootBlocks()*sb.blockSize()/64;

#if DEBUG
	cout << "\n----- ROOTDIR DEBUG -----"  << endl;
	cout << "Start: " << start << endl;
	cout << "Blocks: " << blocks << endl;
	cout << "Entries: " << entries << endl;
	cout << "----- ROOTDIR DEBUG END -----\n" << endl;
#endif 
	int i;
	root = new DirEntries[entries];
	for (i = 0; i < entries; i++) {
		if (fseek(fp, start*512+(64*i), SEEK_SET) == -1) {
			cerr << "seek failed";
			exit(1);
		}
		root[i].getEntry(fp);
	}
}

int RootDir::fileExists(char *fname) {
	int i = 0;
	while (i < entries) {
		if (!strncmp(fname, root[i].filename(), 30))
			return  i;
		i++;
	}
	return -1;
}

void RootDir::getFile(FILE *fp, Superblock sb, char *fname) {
	int index;
	if ((index = fileExists(fname)) < 0) {
		cout << "File not found." << endl;
		return;
	}

	FILE *outfp;
	if ((outfp = fopen(fname, "w")) == NULL) {
		cerr << "Error: failed to write to file: " << fname << endl;
		exit(1);
	}
	unsigned char block_buffer[512];
#if DEBUG
	cout << "\n----- getFile DEBUG -----" << endl;
	cout << root[index].startingBlock() << endl;
	cout << "----- getFile DEBUG END -----\n" << endl;
#endif
	unsigned int i = 0;
	unsigned int next_block;
	unsigned int bs = sb.blockSize();
	unsigned int fs = sb.fatStart();

	next_block = root[index].startingBlock();

	/* while (i < root[index].blockCount()-1) { */
	if (root[index].fileSize() % bs == 0) {
		while (i < root[index].blockCount()) {
			fseek(fp, next_block * bs, SEEK_SET);
			fread(&block_buffer, bs, 1, fp);
			fwrite(&block_buffer, bs, 1, outfp);
			fseek(fp, fs * bs + next_block * 4, SEEK_SET);
			fread(&next_block, 4, 1, fp);
			next_block = htonl(next_block);

			i++;
		}
	} else {
		while (i < root[index].blockCount()-1) {
			fseek(fp, next_block * bs, SEEK_SET);
			fread(&block_buffer, bs, 1, fp);
			fwrite(&block_buffer, bs, 1, outfp);
			fseek(fp, fs * bs + next_block * 4, SEEK_SET);
			fread(&next_block, 4, 1, fp);
			next_block = htonl(next_block);
			i++;
		}
		fseek(fp, next_block * bs, SEEK_SET);
		fread(&block_buffer, root[index].fileSize() % bs, 1, fp);
		fwrite(&block_buffer, root[index].fileSize() % bs, 1, outfp);

	}
}

void RootDir::closeDir() {
	delete [] root;
}

void RootDir::print() {
	int i;
	for (i = 0; i < entries; i++) {
		root[i].print();
	}
}

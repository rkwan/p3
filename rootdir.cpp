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
		root[i].getEntry(fp, sb);
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

int RootDir::availableEntry() {
	int i = 0;
	while (i < entries) {
		if (root[i].status() == 0x00)
			return i;
		i++;
	}
	return -1;
}

int RootDir::availableFAT(FILE *fp, Superblock sb) {
	/* probably more efficient if I have I as a global variable. */
	int i = 0;
	unsigned int buffer;

	int bs = sb.blockSize();
	int s = sb.fatStart();
	int b = sb.fatBlocks();

	fseek(fp, s * bs, SEEK_SET);
	while (i < b*(bs/4)) {
		fread(&buffer, sizeof(unsigned int), 1, fp);
		buffer = htonl(buffer);
		if (buffer == 0x00000000)
			return i;
		i++;
	}
	return -1;	
}

void RootDir::updateFDT(Superblock sb, int index, char s, char *fname) {
	root[index].setStatus(s, index, &sb);
	// FIXME
	root[index].setStartingBlock(80, index, &sb);
	root[index].setBlockCount(1, index, &sb);
	root[index].setFilesize(300, index, &sb);
	root[index].setModifyTime(index, &sb);	
	root[index].setFilename(fname,index, &sb);
}

void RootDir::putFile(FILE *fsfp, char *srcfname, Superblock sb) {
	FILE *srcfp;

	if ((srcfp = fopen(srcfname, "r")) == NULL) {
		cerr << "File not found." << endl;
		exit(1);
	}
	int i;
	int j;
	if ((i = availableEntry()) < 0) {
		cout << "No more directory entries available." << endl;
		return;
	}
#if DEBUG
	cout << i << endl;
#endif
	if ((j = availableFAT(fsfp, sb)) < 0) {
		cout << "Not more FAT entires available." << endl;
		return;
	}
#if DEBUG
	cout << j << endl;
#endif
	updateFDT(sb, i, 3, srcfname);
	fclose(srcfp);
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
	fclose(outfp);
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


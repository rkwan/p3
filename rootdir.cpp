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

void RootDir::updateFDT(Superblock sb, int index, char s, int fb, int bc, int fs, char *fname) {
	root[index].setStatus(s, index, &sb);
	root[index].setStartingBlock(fb, index, &sb);
	root[index].setBlockCount(bc, index, &sb);
	root[index].setFilesize(fs, index, &sb);
	root[index].setModifyTime(index, &sb);
	root[index].setFilename(fname,index, &sb);
}

int RootDir::findNextAvailableFat(FILE *fp, Superblock *sb, int startAt) {
	unsigned int buffer;
	int s = startAt;
	fseek(fp, sb->fatStart()*sb->blockSize() + (startAt+1)*4, SEEK_SET);
	startAt++;
	fread(&buffer, sizeof(unsigned int), 1, fp);
	while (buffer != 0x00000000) {
		startAt++;
		fread(&buffer, sizeof(unsigned int), 1, fp);
	}
	/* undoing the side effect. */
	fseek(fp, sb->fatStart()*sb->blockSize() + s*4, SEEK_SET);
	return startAt;
}

void RootDir::putFile(FILE *fsfp, char *srcfname, Superblock sb) {
	FILE *srcfp;
	if ((srcfp = fopen(srcfname, "r")) == NULL) {
		cerr << "File not found." << endl;
		exit(1);
	}

	int index;
	int status = 3;
	int firstblock;
	int blockcount = 0;
	int filesize = 0;

	if ((index = availableEntry()) < 0) {
		cout << "No more directory entries available." << endl;
		return;
	}
	if ((firstblock = availableFAT(fsfp, sb)) < 0) {
		cout << "No more FAT entires available." << endl;
		return;
	}
#if DEBUG
	cout << "\n----- putFile DEBUG -----" << endl;
	cout << "Index: " << index << endl;
	cout << "First Block: " << firstblock << endl;
	cout << "----- putFile DEBUG END -----\n" << endl;
#endif

	char buffer[sb.blockSize()];
	int next_available = firstblock;
	int leftover;

	fseek(srcfp, 0, SEEK_SET);
	fseek(fsfp, sb.blockSize()*firstblock, SEEK_SET);
	while ((leftover = fread(&buffer, sb.blockSize(), 1, srcfp)) == 1) {
		/* probably would be nice to have a function to check */
		/* whether there are free fat entries left before writing. */
		fwrite(&buffer, sb.blockSize(), 1, fsfp);
		filesize += sb.blockSize();
		blockcount++;

		fseek(fsfp, sb.fatStart()*sb.blockSize() + next_available*4, SEEK_CUR);
		next_available = htonl(findNextAvailableFat(fsfp, &sb, next_available));
		fwrite(&next_available, 4, 1, fsfp);
		next_available = htonl(next_available);
		fseek(fsfp, sb.blockSize()*next_available, SEEK_SET);

	}
	unsigned int i;
	char byte;
	if (leftover < 1) {
		i = 0;
		fseek(srcfp, blockcount*sb.blockSize(), SEEK_SET);
		while (fread(&byte, 1, 1, srcfp)) {
			fwrite(&buffer[i], 1, 1, fsfp);
			filesize++;
			i++;
		}
		blockcount++;
	}
	unsigned int endoffile = 0xffffffff;
	fseek(fsfp,sb.fatStart()*sb.blockSize() + next_available*4, SEEK_CUR);
	fwrite(&endoffile, 4, 1, fsfp);


	updateFDT(sb, index, status, firstblock, blockcount, filesize, srcfname);
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


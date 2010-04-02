#include <stdio.h>
#include "fat.h"

FAT::FAT(Superblock sb, FILE *fp) {
	unsigned int i, buffer;
	free_blocks = 0;
	reserved_blocks = 0;
	allocated_blocks = 0;
	last_blocks = 0;

	fseek(fp, sb.fatStart()*sb.blockSize(), SEEK_SET);
	for (i = 0; i < sb.fatBlocks()*(sb.blockSize()/4); i++) {
		fread(&buffer, 4, 1, fp);
		buffer = htonl(buffer);
		if (buffer == 0x00000000)
			free_blocks++;
		else if (buffer == 0x00000001)
			reserved_blocks++;
		else if (buffer == 0xffffffff)
			last_blocks++;
		else
			allocated_blocks++;
	}
}

void FAT::print() {
	cout << "FAT information:" << endl;
	cout << "Free Blocks: " << free_blocks << endl;
	cout << "Reserved Blocks: " << reserved_blocks << endl;
	cout << "Allocated Blocks: " << allocated_blocks << endl;
#if DEBUG
	cout << "Last Blocks: " << last_blocks << endl; // Remember to remove this before submitting.
#endif
}

#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include "superblock.h"
#include "direntries.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Error: invalid arguments\n");
		exit(1);
	}
	FILE *fp;
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error: failed to open file\n");
		exit(1);
	}
	Superblock sb (fp);
	fseek(fp, sb.rootStart()*sb.blockSize(), SEEK_SET);
	DirEntries dir (fp);
	dir.print();
	return 0;
}

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "superblock.h"
#include "fat.h"
#include "rootdir.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "Error: invalid arguments" << endl;
		exit(1);
	}
	FILE *fp;
	FILE *outfp;
	if ((fp = fopen(argv[1], "r")) == NULL) {
		cerr << "Error: failed to open file: " << argv[1];
		exit(1);
	}
	if ((outfp = fopen(argv[2], "w")) == NULL) {
		cerr << "Error: failed to open file: " << argv[2];
		exit(1);
	}
	unsigned char block[512];
	Superblock sb(fp);
	FAT filealloctable(sb, fp);
	/* might have to change the parameters so it is consistant. */
	RootDir root(fp, sb); 
	/* add a function in rootdir that will return the location of the file. */
	return 0;
}

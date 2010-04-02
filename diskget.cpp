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
	if ((fp = fopen(argv[1], "r")) == NULL) {
		cerr << "Error: failed to open file: " << argv[1];
		exit(1);
	}
	Superblock sb(fp);

	RootDir root(fp, sb); 
	root.getFile(fp, sb, argv[2]);
	root.closeDir();
	fclose(fp);
	return 0;
}

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
	FILE *fsfp;

	if ((fsfp = fopen(argv[1], "r+b")) == NULL) {
		cerr << "Error: failed to open file: " << argv[1] << endl;
		exit(1);
	}

	Superblock sb(fsfp);
	RootDir root(fsfp, sb);
	root.putFile(fsfp, argv[2], sb);
	root.closeDir();
	fclose(fsfp);

	return 0;
}

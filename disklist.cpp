#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include "fat.h"
#include "superblock.h"

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

	return 0;
}

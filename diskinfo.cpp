#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "superblock.h"
#include "fat.h"

using namespace std;

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        fprintf(stderr, "Error: invalid argument(s)");
        exit(1);
    }
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: failed to open file: %s", argv[1]);
        exit(1);
    }
    Superblock sb (fp);
    FAT f (sb, fp);
    sb.print();
    cout << endl;
    f.print();
    fclose(fp);
    return 0;
}

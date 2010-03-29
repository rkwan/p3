#include <stdlib.h>
#include "direntries.h"

DirEntries::DirEntries(FILE *fp) {
	unsigned int buffer;

	fseek(fp, 0, SEEK_CUR);
	fread(&_status, sizeof(unsigned char), 1, fp);
	fread(&buffer, sizeof(unsigned int), 1, fp);
	starting_block = htonl(buffer);
	fread(&buffer, sizeof(unsigned int), 1, fp);
	block_count = htonl(buffer);
	fread(&buffer, sizeof(unsigned int), 1, fp);
	file_size = htonl(buffer);
	fread(&create_time, 7, 1, fp);
	fread(&modify_time, 7, 1, fp);
	fread(&file_name, 30, 1, fp);
	file_name[30] = '\0';
}

void DirEntries::print() {
	if ( _status == 3 || _status == 5) {
		if ( _status == 3)
			cout << "F ";
		else if ( _status == 5)
			cout << "D ";

		cout << setw(10) << file_size;
		cout << setw(30) << file_name << endl;
		/* #if DEBUG */
		/* cout << create_time; */
		/* #endif */
		/* cout << modify_time; */
	}
}

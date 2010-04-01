#include <stdlib.h>
#include <string.h>
#include "direntries.h"

DirEntries::DirEntries() {
	_status = 0;
	starting_block = 0;
	file_size = 0;
}

void DirEntries::getEntry(FILE *fp) {
	unsigned int buffer;

	/* fseek(fp, 0, SEEK_CUR); */
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
		unsigned short int year;
		unsigned char month;
		unsigned char day;
		unsigned char hour;
		unsigned char minute;
		unsigned char second;

		memcpy(&year, &modify_time, 2);
		memcpy(&month, &modify_time[2], 1);
		memcpy(&day, &modify_time[3], 1);
		memcpy(&hour, &modify_time[4], 1);
		memcpy(&minute, &modify_time[5], 1);
		memcpy(&second, &modify_time[6], 1);

		if ( _status == 3)
			cout << "F ";
		else if ( _status == 5)
			cout << "D ";

		cout << setw(10) << file_size << " ";
		cout << setw(30) << file_name << " ";

		cout << htons(year) << "/";
		if (month < 10)
			cout << "0";
		cout << (int) month << "/";
		if (day < 10)
			cout << "0";
		cout << (int) day << " ";
		if (hour < 10)
			cout << "0";
		cout << (int) hour << ":";
		if (minute < 10)
			cout << "0";
		cout << (int) minute << ":";
		if (second < 10)
			cout << "0";
		cout << (int) second << endl;
#if DEBUG
		cout << "\n----- DIRENTRIES DEBUG -----" << endl;
		cout << "Starting Block: " << starting_block << endl;
		cout << "Block Count: " << block_count << endl;
		cout << "----- DIRENTRIES DEBUG END -----\n" << endl;
#endif
	}
}

#include "direntries.h"

DirEntries::DirEntries() {
	_status = 0;
	starting_block = 0;
	file_size = 0;
}

void DirEntries::getEntry(FILE *fp, Superblock superblock) {
	unsigned int buffer;

	fptr = fp;

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

void DirEntries::setStatus(char s, int index, Superblock *sb) {
	_status = s;

	int offset = 0;
	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(&s, 1, 1, fptr);
}

void DirEntries::setStartingBlock(unsigned int startb, int index, Superblock *sb) {
	starting_block = startb;

	int offset = 1;
	startb = htonl(startb);
	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(&startb, 4, 1, fptr);
}

void DirEntries::setBlockCount(unsigned int bc, int index, Superblock *sb) {
	block_count = bc;	

	int offset = 5;
	bc = htonl(bc);
	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(&bc, 4, 1, fptr);
}

void DirEntries::setFilesize(unsigned int fsize, int index, Superblock *sb) {
	file_size = fsize;

	int offset = 9;
	fsize = htonl(fsize);
	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(&fsize, 4, 1, fptr);
}

void DirEntries::setModifyTime(int index, Superblock *sb) {
	int offset = 20;
	unsigned short int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;

	time_t rawtime;
	tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	year = htons((short int) timeinfo->tm_year + 1900);
	month = (unsigned char) timeinfo->tm_mon + 1;
	day = (unsigned char) timeinfo->tm_mday;
	hour = (unsigned char) timeinfo->tm_hour;
	minute = (unsigned char) timeinfo->tm_min;
	second = (unsigned char) timeinfo->tm_sec;

	memcpy(&modify_time[0], &year, 2);
	memcpy(&modify_time[2], &month, 1);
	memcpy(&modify_time[3], &day, 1);
	memcpy(&modify_time[4], &hour, 1);
	memcpy(&modify_time[5], &minute, 1);
	memcpy(&modify_time[6], &second, 1);

	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(&year, 2, 1, fptr);
	fwrite(&month, 1, 1, fptr);
	fwrite(&day, 1, 1, fptr);
	fwrite(&hour, 1, 1, fptr);
	fwrite(&minute ,1, 1, fptr);
	fwrite(&second, 1, 1, fptr);
}

void DirEntries::setFilename(char *fname, int index, Superblock *sb) {
	strncpy(file_name, fname, 30);
	int offset = 27;
	fseek(fptr, offset + sb->blockSize()*sb->rootStart() + index*64, SEEK_SET);
	fwrite(fname, 30, 1, fptr);
}

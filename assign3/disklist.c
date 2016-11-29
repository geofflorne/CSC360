#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

unsigned char * process_name(unsigned char* name){
	int i, j;
	unsigned char * new_s = (unsigned char *) malloc(11);

	for(i = 0; i < 8; i++){
		if(name[i] != ' ')
			new_s[i] = name[i];
		else
			break;
	}

	new_s[i] = '.';

	for(j = 8; j < 11; j++){
		if(name[j] != ' ')
			new_s[++i] = name[j];
		else
			break;
	}

	return new_s;
}

void lst(unsigned char *addr){
	int bytes_per_sector = addr[11] + (addr[12]<<8);
	int root = bytes_per_sector * 19;
	int size;
	char type;
	unsigned char * name;
	int c_date, year, month, day;
	int c_time, hour, minute, second;

	while(addr[root] != 0){
		if((addr[root + 8 + 3] & 0x10) == 0x10)
			type = 'D';
		else
			type = 'F';

		if(addr[root + 8 + 3] != 0x0f && addr[root + 8 + 3] != 0x08){

			size  =  addr[root + 28] + (addr[root + 29]<<8) + (addr[root + 30]<<16) + (addr[root + 31]<<24);
			name   =  addr + root;

			if(type == 'F') name = process_name(name);

			c_date =  addr[root + 16] + (addr[root + 17]<<8);
			year  = ((c_date & 0xfe00) >> 9) + 1980;
			month = ((c_date & 0x01e0) >> 5);
			day   = ((c_date & 0x001f));

			c_time =  addr[root + 14] + (addr[root + 15]<<8);
			hour   = ((c_time & 0xf800) >> 11);
			minute = ((c_time & 0x07e0) >> 5);
			second = ((c_time & 0x001f)) * 2;

			printf("%c ", type);
			printf("%10u ", size);
			printf("%20s ", name);
			printf("%02d-%02d-%02d ", year, month, day);
			printf("%02d:%02d:%02d ", hour, minute, second);
			printf("\n");
		}

		root += 32;
	}
}

int main(int argc, char *argv[]){

	unsigned char *addr;
	int fd;
	struct stat sb;

	fd = open(argv[1], O_RDONLY);
	if(fd < 0 ){
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}

	if(fstat(fd, &sb) < 0){
		printf("Error getting file stats\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	lst(addr);
	munmap(addr, sb.st_size);
	close(fd);
	return 0;
}

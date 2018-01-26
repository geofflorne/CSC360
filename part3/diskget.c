// open disk file
// get disk size
// char *src = mmap(disk file, ... disk size)
//
// check for file to be copied in disk root dir, grab its file size & related info
// open a file in current directory with same size
// char *dest = mmap(new file, ... file size)
// copy file from src->dest, reading sector by sector
//
// munmap(disk file)
// munmap(file)
//
// close disk file
// close file

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

int find_file(char *addr, char *file_name){
	int bytes_per_sector = addr[11] + (addr[12]<<8);
	int root = bytes_per_sector * 19;
	while(addr[root] != 0){
		if(strcmp(process_name(addr+root), file_name) == 0){
			return root;
		}
		root += 32;
	}
	return -1;
}

void get_file(unsigned char *addr, char *file_name){
	int i = find_file(addr,file_name);
	char *name = process_name(addr+i);
	int size = addr[i + 28] + (addr[i + 29]<<8) + (addr[i + 30]<<16) + (addr[i + 31]<<24);
	int fd = open(name, O_WRONLY);
	unsigned char *dest = mmap(NULL, size, PROT_WRITE, MAP_PRIVATE, fd, 0);


	munmap(dest, size);
	close(fd);
}

int get_FAT_value(unsigned char* addr, int n, int offset){
	int left, right;

	left = addr[(offset + (3 * n / 2))];
	right  = addr[(offset + (1 + 3 * n / 2))];
	if(n%2 == 0){
		//low 4 bits of posistion 1+(3*n)/2 and 8 bits of posistion (3*n)/2
		return ((left ) + ((right & 0x0f)<< 8));
	}else{
		//8 bits of posistion 1+(3*n)/2 and high bits of posistion (3*n)/2
		return (((left & 0xf0) >> 4)) + (right << 4);
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

	get_file(addr, argv[2]);

	munmap(addr, sb.st_size);
	close(fd);
	return 0;
}

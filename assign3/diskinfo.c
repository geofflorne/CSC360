#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>



int get_FAT_value(int n){
	if(n%2 == 0){
		return (1+(3*n)/2) + (((3*n)/2)<<8);
		}else{
		return (1+(3*n)/2) + (((3*n)/2)<<4);
	}
}

char *volume_label(char *addr){
	int bytes_per_sector = addr[11] + (addr[12]<<8);
	int root = bytes_per_sector * 19;
	int count = 0;
	while(addr[root] != 0){
		if(addr[root + 8 + 3] == 8)
			return addr + root;
		root += 32;
	}
	return "Could not find volume_label";
}

int num_root_dirs(char *addr){
	int bytes_per_sector = addr[11] + (addr[12]<<8);
	int root = bytes_per_sector * 19;
	int count = 0;
	while(addr[root] != 0){
		printf("%s      ", addr + root);
		printf("%d\n", (addr[root + 8 + 3] & 0x10));
		//if(addr[root + 8 + 3] & 0x10 != 0x10)
			count++;
		root += 32;
	}
	return count;
}

int main(int argc, char *argv[]){
	char *addr;
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

	printf("OS Name: %s\n", addr + 3);
	printf("Label of the disk: %s\n",volume_label(addr));
	printf("Total size of the disk: %ld bytes\n", sb.st_size);
	//printf("Free size of the disk: %ld bytes\n", sb.st_size);
	printf("\n==============\n");
	printf("The number of files in the root directory (not including subdirectories): %d\n", num_root_dirs(addr));
	printf("\n==============\n");
	printf("Number of FAT copies: %d\n", addr[16]);
	printf("Sectors per FAT : %d\n", addr[22] + (addr[23]<<8));

	//printf("Bytes per sector: %d\n", addr[11] + (addr[12]<<8));

	munmap(addr, sb.st_size);
	close(fd);
	return 0;
}

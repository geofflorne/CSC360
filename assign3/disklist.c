#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

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




	munmap(addr, sb.st_size);
	close(fd);
	return 0;
}

#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t fpid;
	int count = 0;
 
	fpid = fork();
 
	if (fpid < 0) // fork error
		printf("error in fork!\n");
	else if (fpid == 0) //in child process
	{
		printf("i'm the child process, my pid is %d, my parent's pid is %d\n", getpid(), getppid());
		count++;
	}
	else // in parent process
	{
		printf("i'm the parent process, my pid is %d, my child's pid is %d\n", getpid(), fpid);
		count++;
	}
	printf("count: %d\n", count);
	return 0;
}

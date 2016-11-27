#include <unistd.h>
#include <stdio.h>

int main()
{
	int i = 0;
	printf("i  ch/p  ppid  pid  fpid\n");
	//ppid is the pid of current process's parent
	//pid is the pid of current process
	//fpid is the returned value by calling fork()
	for(i = 0; i < 2; i++)
	{
		pid_t fpid = fork();
		if (fpid < 0) //fork error
			printf("error in fork!\n");
		else if(fpid == 0) // in child process
			printf("%d child  %4d %4d %4d\n", i, getppid(), getpid(), fpid);
		else // in parent process
			printf("%d parent %4d %4d %4d\n", i, getppid(), getpid(), fpid);
	}
    sleep(5000);
	return 0;
}

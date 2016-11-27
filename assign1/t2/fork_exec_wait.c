#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	if (pid == 0) //child process
	{
		sleep(5);
		execlp("ls", "ls", NULL);
	} else
	{
        pid_t pid2 = fork();
	    if (pid2 == 0) //child process
	    {
		    sleep(2);
		    execlp("./hello", NULL);
        } else {
                wait(NULL);
            	printf("Child Complete!\n");
               	sleep(2);
	        	printf("Parent Exit!\n");
	        	exit(0);
        }
    }	
	
	
}

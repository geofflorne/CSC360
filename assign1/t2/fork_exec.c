# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>

int main(void)
{
	char *const parmList[] = {"ls", "-l", NULL};
	if (fork() == 0){
        if (execv("/bin/ls", parmList) < 0)
            perror("Error on execv.");
    }
    else {
        wait(NULL);
        printf("parent\n");
    }
    printf("finish\n");
	return 0;
}

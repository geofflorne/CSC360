#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>

void main(){
    FILE     *fp;
    char path[1024];
    fp = popen("ls -l", "r");
    if(fp) {
        while(fgets(path, sizeof(path)-1, fp) != NULL){
            printf("%s\n", path);
            char *str; 
            str = strtok(path, " ");
            
            while (str != NULL){
                printf("%s\n", str);
                str = strtok(NULL, " "); 
            }
        } 
    } else {
        perror("popen error");
        exit(1);
    }
    pclose(fp);

}


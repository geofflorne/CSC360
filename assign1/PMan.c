#include <unistd.h>             // fork(), execvp()
#include <stdio.h>              // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>             // malloc()
#include <sys/types.h>          // pid_t
#include <sys/wait.h>           // waitpid()
#include <signal.h>             // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <errno.h>              // errno
#include <string.h>             //strtok()
#include <readline/readline.h>  //readline()
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "proc.h"

#define ARGLIMIT 100

void update_bg_procss() {

    //use while loop to monitor all process with -1 option.
    while(1) {
    id = waitpid(-1, status, WNOHANG | WUNTRACED | WCONTINUED) to monitor the state of all child process
    if id = 0 ; none of children changed state, no proc_list update needed. break while loop.
    if id > 0 ;
        1 if(WIFSIGNALED(status)) got killed. Inform user that process [id] terminated. remove the item where pid = id in proc_list.
        2 if(WIFSTOPPED(status)) got stopped. set isStop = 1
        3 if(WIFCONTINUED(status)) stopped process continues, set isStop back to 0.
    if id < 0 :
        no more updates, break while loop.
    }

    return ;
}

int main(){

    proc *ptr = NULL;
    char *input = NULL;
    char *prompt = "PMan: > ";
    char *tok;
    int   pid;
    char *args[ARGLIMIT];
    int   i;

    while(1) {

        //update_bg_process();

        input = readline(prompt);
        tok = strtok(input, " ");
        //tok = strtok(NULL, " ");
        if (strcmp(tok, "pstat") == 0){
            //     FILE *fp = popen("/proc/[pid]/stat(status)", "r") ;
            //     read file content using fgets. (see pipe.c)
            //     output variables listed in assign.
        }else if (strcmp(tok, "bg") == 0){
            i = 0;
            //     1. pid = fork();
            //     2. in child proces :  exec*(cmd)
            //     3. create a new item in proc_list using malloc().
            //     4. store pid, cmd, istStop = 0 in new item.
            //     5. add new item in proc_list.
            //tok = strtok(NULL, "\0");
            pid = fork();
            if (pid > 0){
              tok = strtok(NULL, " ");
              while(tok != NULL){
                args[i++] = tok;
                tok = strtok(NULL, " ");
              }
              args[i++] = NULL;
              execvp(args[0], args);
              add(pid, args[0], 0);

            }else if (pid == 0){

            }else{
                printf("fork failed\n");
                //error
            }


        }else if (strcmp(tok, "bgkill") == 0){
            //     handle signal using kill() ;
        }else if (strcmp(tok, "bgstart") == 0){
            //     handle signal using kill() ;
        }else if (strcmp(tok, "bgstop") == 0){
            //     handle signal using kill() ;
        }else if (strcmp(tok, "bglist") == 0){
            //     list all items in proc_list where isStop = 0 ;
        }else {
          printf("%s: command not found\n", input);
        }


    }
    return 1;
}
